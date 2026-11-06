/**
  ******************************************************************************
  * @file    debug_authentication.c
  * @author  MCD Application Team
  * @brief   This file provides all debug authentication functions.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx.h"
#include "bootutil_priv.h"
#include "bootutil/crypto/sha256.h"
#include "low_level_otp.h"
#include "debug_authentication.h"

/* Private defines -----------------------------------------------------------*/
#define DEBUG_REQUEST_MESSAGE "OEDA"
#define DEBUG_REPLY_MESSAGE "DAOK"

#define OTP_UID 5               /* OTP 5 : 96-bit unique ID */
#define OTP_BOOTROM_CONFIG_9 18 /* OTP 18 : BOOTROM configuration 9 contains secure_boot and prov_done */

#define DA_TIMEOUT 0xFFFFFFFF   /* Infinite timeout */

#define PASSWORD_LENGTH_MIN 4   /* Password minimum length */
#define PASSWORD_LENGTH_MAX 16  /* Password maximum length */
#define BUFFER_SIZE 256         /* Buffer size that holds TLVs */
#define TLV_BUFFER_SIZE 64      /* TLV buffer size in bytes */
#define RESPONSE_BUFFER_SIZE 10 /* TLV buffer size in bytes */

#define SHA256_LEN_BYTES 0x20   /* SHA-256 hash length in bytes (32 bytes) */
#define SHA256_LEN_WORDS 0x08   /* SHA-256 hash length in words (8 words) */

#define CHALLENGE_VECTOR_SIZE 0x20  /* Challenge vector size in bytes (32 bytes) */

/* Private typedef -----------------------------------------------------------*/
/** \brief TLV type for extensions
  *
  */
typedef struct
{
  uint16_t _reserved; //!< Must be set to zero.
  uint16_t type_id;
  uint32_t length_in_bytes;
  uint8_t value[TLV_BUFFER_SIZE];
} psa_tlv_t;

typedef struct {
    uint16_t _reserved; //!< Must be set to zero.
    uint16_t status;
    uint32_t data_count;
    uint32_t data[RESPONSE_BUFFER_SIZE];
} response_packet_t;

/** \brief Version type
  *
  */
typedef struct
{
  uint8_t major;
  uint8_t minor;
} psa_version_t;

/** \brief Authentication challenge
  *
  */
typedef struct
{
  psa_version_t format_version;
  uint16_t _reserved;
  uint8_t challenge_vector[CHALLENGE_VECTOR_SIZE];
} psa_auth_challenge_t;

/** \brief Commands
  *
  */
typedef enum
{
  SDP_DISCOVERY_CMD = 0x01U,      /**< `Discovery` command */
  SDP_AUTH_START_CMD = 0x02U,     /**< `Start Authentication` command */
  SDP_AUTH_RESPONSE_CMD = 0x03U,  /**< `Authentication Response` command */
} sdp_commands_t;

typedef enum {
    NULL_TYPE             = 0x0000,
    ADAC_AUTH_VERSION     = 0x0001,
    VENDOR_ID             = 0x0002,
    SOC_CLASS             = 0x0003,
    SOC_ID                = 0x0004,
    HW_PERMISSIONS_MASK   = 0x0007,
    PSA_LIFECYCLE         = 0x0008,
    SDA_ID                = 0x000A,
    TOKEN_FORMATS         = 0x0100,
    CERT_FORMATS          = 0x0101,
    CRYPTOSYSTEMS         = 0x0102,
    PSA_ST_PASSWD         = 0x8000,
    HDPL1_STATUS          = 0x8001,
    HDPL2_STATUS          = 0x8002,
    HDPL3_STATUS          = 0x8003
} type_id_t;


/*!
 * @brief Parameters outputted by SDMDiscovery through Debugger.
 */
typedef struct SDMDiscoveryParameters {
    uint32_t targetID;                  /*!< SoC Class. */
    uint16_t tokenFormats;              /*!< Token Formats. */
    uint16_t certifFormats;             /*!< Certificate Formats. */
    uint32_t provIntegStatus;           /*!< ST provisioning integrity status. */
    uint32_t hdpl1Status;               /*!< ST HDPL1 status. */
    uint32_t hdpl2Status;               /*!< ST HDPL2 status. */
    uint32_t hdpl3Status;               /*!< ST HDPL3 status. */
    uint32_t socID[4];                  /*!< SoC ID. */
    char lifeCycle[2];                  /*!< PSA lifecycle. */
    char cryptosystems;                 /*!< Cryptosystems. */
    char vendorID[2];                   /*!< Vendor ID. */
    char psaAuthVer[2];                 /*!< PSA auth version. */
    char sdaVer[4];                     /*!< SDA version. */
} SDMDiscoveryParameters;

/* Global variables ----------------------------------------------------------*/
SDMDiscoveryParameters DiscoveryParameters = {
.targetID = 0,
.tokenFormats = 0,
.certifFormats = 0,
.provIntegStatus = 0xffffffff,
.hdpl1Status = 0x11111111,
.hdpl2Status = 0x22222222,
.hdpl3Status = 0x33333333,
.socID = {0},
.lifeCycle = {0x17, 0x20},
.cryptosystems = 0x80,
.vendorID = {0x20, 0},
.psaAuthVer = {1, 0},
.sdaVer = {0x02, 0x04, 0x00, 0x00},
};

static uint8_t buff[BUFFER_SIZE] = {0};
static uint32_t byte_count = 0;
static uint8_t da_password_provisioned = 0;

/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef NonSec_Dbg_AuthConfig(uint32_t NonSec_Dbg_Auth);
static HAL_StatusTypeDef OTP_Get_Password_Hash(uint32_t* hash);

static HAL_StatusTypeDef send_discovery_message(void);
static void add_tlv(uint16_t id, uint8_t *buffer, uint32_t length_in_bytes);
static HAL_StatusTypeDef send_message(uint32_t *message, uint32_t length, uint32_t timeout);
static HAL_StatusTypeDef receive_message(uint32_t *buffer, uint32_t length, uint32_t timeout);
static HAL_StatusTypeDef receive_word(uint32_t *word, uint32_t timeout);

static void DBGMCU_DBG_AUTH_DEVICE_WRITE(uint32_t message);
static uint32_t DBGMCU_DBG_AUTH_HOST_READ(void);
static uint32_t DBGMCU_DBG_AUTH_DEVICE_READ_ACK(void);
static void DBGMCU_DBG_AUTH_HOST_WRITE_ACK(void);
static uint32_t DBGMCU_DBG_AUTH_HOST_READ_ACK(void);
static void DBGMCU_DBG_ENABLE(void);

/* Functions Definition ------------------------------------------------------*/
/**
  * @brief Conduct debug authentication process.
  *
  * This function handle communication with secure debug manager.
  * The purpose is to allow non secure debug if the communicated
  * password is correct.
  */
HAL_StatusTypeDef debug_authentication(void)
{
  uint32_t buffer = 0, message = 0;
  psa_tlv_t psa_tlv = {0};
  bootutil_sha256_context sha256_ctx = {0};
  uint32_t password_length = 0;
  psa_auth_challenge_t psa_auth_challenge = {0};

  response_packet_t rep = {0};
  HAL_StatusTypeDef status = HAL_ERROR;
  fih_int fih_rc = FIH_FAILURE;

  uint8_t computed_hash[SHA256_LEN_BYTES] = {0};
  uint32_t stored_hash[SHA256_LEN_WORDS] = {0};

  /* Enable DBGMCU */
  DBGMCU_DBG_ENABLE();

  /* Check debug request message */
  status = receive_message(&buffer, sizeof(buffer), 0);
  message = (DEBUG_REQUEST_MESSAGE[3]) |
            (DEBUG_REQUEST_MESSAGE[2] << 8) |
            (DEBUG_REQUEST_MESSAGE[1] << 16) |
            (DEBUG_REQUEST_MESSAGE[0] << 24);
  if (status == HAL_OK && buffer == message)
  {
    /* Send reply */
    message = (DEBUG_REPLY_MESSAGE[3]) |
              (DEBUG_REPLY_MESSAGE[2] << 8) |
              (DEBUG_REPLY_MESSAGE[1] << 16) |
              (DEBUG_REPLY_MESSAGE[0] << 24);
    status = send_message(&message, sizeof(buffer), DA_TIMEOUT);
    if (status == HAL_OK)
    {
      /* Receive discovery request message */
      status = receive_message((uint32_t*)&psa_tlv, 8, DA_TIMEOUT);
      if (status == HAL_OK && psa_tlv.type_id == SDP_DISCOVERY_CMD)
      {
        /* Send discovery message reply */
        status = send_discovery_message();
        if (status == HAL_OK)
        {
          /* Receive challenge request */
          status = receive_message((uint32_t*)&psa_tlv, 8, DA_TIMEOUT);
          if (status == HAL_OK && psa_tlv.type_id == SDP_AUTH_START_CMD)
          {
            /* Send challenge */
            rep.data_count = sizeof(psa_auth_challenge) / 4; /* Convert bytes to words */
            memcpy(&rep.data, &psa_auth_challenge, sizeof(psa_auth_challenge));
            status = send_message((uint32_t*)&rep, 8 + sizeof(psa_auth_challenge), DA_TIMEOUT);
            if (status == HAL_OK)
            {
              /* Receive password length */
              status = receive_message((uint32_t*)&psa_tlv, 8, DA_TIMEOUT);
              password_length = 4 * psa_tlv.length_in_bytes; /* Convert to bytes */
              password_length -= 8; /* Subtract psa tlv length */
              if (status == HAL_OK &&
                  psa_tlv.type_id == SDP_AUTH_RESPONSE_CMD &&
                  password_length >= PASSWORD_LENGTH_MIN &&
                  password_length <= PASSWORD_LENGTH_MAX)
              {
                /* Receive password */
                status = receive_message((uint32_t*)&psa_tlv, 4 * psa_tlv.length_in_bytes, DA_TIMEOUT);
                if (status == HAL_OK && psa_tlv.type_id == PSA_ST_PASSWD)
                {
                  /* Compute received password hash */
                  bootutil_sha256_init(&sha256_ctx);
                  bootutil_sha256_update(&sha256_ctx, psa_tlv.value, psa_tlv.length_in_bytes);
                  bootutil_sha256_finish(&sha256_ctx, computed_hash);

                  memset(&rep, 0, sizeof(rep));
                  rep.status = 1;

                  /* Retrieve password hash stored in OTP */
                  if (OTP_Get_Password_Hash(stored_hash) == HAL_OK)
                  {
                    /* Compare between received password hash and stored hash */
                    fih_rc = boot_fih_memequal(stored_hash, computed_hash, SHA256_LEN_BYTES);
                    if (fih_eq(fih_rc, FIH_SUCCESS))
                    {
                      /* Set DBG authorizations */
                      NonSec_Dbg_AuthConfig(HAL_BSEC_NONSEC_DBG_AUTH);
                      rep.status = 0;
                    }
                  }
                  /* Send confirmation */
                  status = send_message((uint32_t*)&rep, 8, DA_TIMEOUT);
                }
              }
            }
          }
        }
      }
    }
  }

  return status;
}

/**
 * @brief Retrieve Lifecycle Status
 *
 * This function reads the One-Time Programmable (OTP) memory to determine
 * the lifecycle status of the device.
 *
 * @param[out] secure_boot Pointer to a variable where the secure boot status will be stored.
 *                         - 0: Secure boot is not enabled
 *                         - > 0: Secure boot is enabled
 * @param[out] prov_done   Pointer to a variable where the provisioning status will be stored.
 *                         - 0: Provisioning is not done
 *                         - > 0: Provisioning is done
 *
 * @return HAL_StatusTypeDef - status of the operation.
 */
HAL_StatusTypeDef OTP_Get_Lifecycle_Status(uint8_t* secure_boot, uint8_t* prov_done)
{
  BSEC_HandleTypeDef sBsecHandler = {.Instance = BSEC};
  HAL_StatusTypeDef status = HAL_ERROR;
  uint32_t otp_value = 0;

  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  status = HAL_BSEC_OTP_Read(&sBsecHandler, OTP_BOOTROM_CONFIG_9, &otp_value);
  if (status != HAL_OK)
  {
    return status;
  }

  *secure_boot = otp_value & 0x00000003;
  *prov_done = (otp_value & 0x000001C0) >> 5;

  return HAL_OK;
}

/**
  * @brief Read DA password hash from OTP words.
  *
  * This function reads the One-Time Programmable (OTP) memory to retrieve
  * the debug authentication password hash.
  * @param hash Pointer to the buffer where the hash will be stored.
  * @return HAL_StatusTypeDef - status of the operation.
  */
static HAL_StatusTypeDef OTP_Get_Password_Hash(uint32_t* hash)
{
  BSEC_HandleTypeDef sBsecHandler = {.Instance = BSEC};
  HAL_StatusTypeDef status = HAL_ERROR;
  uint32_t otp_value = 0;
  uint32_t i = 0;

  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* Read password hash located in OTP */
  for (i = 0; i < SHA256_LEN_WORDS; i++)
  {
    status = HAL_BSEC_OTP_Read(&sBsecHandler, OTP_DEBUG_AUTH_PASSWORD_HASH_NUMBER + i, &otp_value);
    if (status != HAL_OK)
    {
      return status;
    }
    /* Reverse endianness */
    if (hash != NULL)
    {
      hash[i] = __REV(otp_value);
    }

    if (otp_value != 0xFF)
    {
      da_password_provisioned = 1;
    }
  }

  return HAL_OK;
}

/**
  * @brief Get the authorization of non secure debug
  *
  * @param NonSec_Dbg_Auth Pointer to a variable where the status
  *                        of non secure debug authorization is stored
  * @return HAL_StatusTypeDef - status of the operation.
  */
static HAL_StatusTypeDef Get_NonSec_Dbg_Auth(uint32_t* NonSec_Dbg_Auth)
{
  BSEC_HandleTypeDef sBsecHandler = {.Instance = BSEC};
  HAL_StatusTypeDef status = HAL_ERROR;
  BSEC_DebugCfgTypeDef DbgCfg = {0};

  status = HAL_BSEC_GetDebugConfig(&sBsecHandler, &DbgCfg);

  *NonSec_Dbg_Auth = DbgCfg.NonSec_Dbg_Auth;
  return status;
}

/* Authorise non secure debugging */
/**
  * @brief Set the authorization of non secure debug
  *
  * @param NonSec_Dbg_Auth Status status of non secure debug authorization to set
  * @return HAL_StatusTypeDef - status of the operation.
  */
static HAL_StatusTypeDef NonSec_Dbg_AuthConfig(uint32_t NonSec_Dbg_Auth)
{
  BSEC_HandleTypeDef sBsecHandler = {.Instance = BSEC};
  BSEC_DebugCfgTypeDef DbgCfg = {0};

  /* Enable BSEC & SYSCFG clocks to ensure BSEC data accesses */
  __HAL_RCC_BSEC_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* Set the BSEC configuration with NS debug authorized */
  DbgCfg.HDPL_Open_Dbg = HAL_BSEC_OPEN_DBG_LEVEL_2;
  DbgCfg.Sec_Dbg_Auth = HAL_BSEC_SEC_DBG_UNAUTH;
  DbgCfg.NonSec_Dbg_Auth = NonSec_Dbg_Auth;

  /* Note to Secure firmware: Do not attempt further writes to BSEC_DBGCR */
  HAL_BSEC_UnlockDebug(&sBsecHandler);
  HAL_BSEC_ConfigDebug(&sBsecHandler, &DbgCfg);

  return HAL_OK;
}

/**
  * @brief Send discovery message using DBGMCU mailbox
  *
  * @return HAL_StatusTypeDef - status of the operation.
  */
static HAL_StatusTypeDef send_discovery_message(void)
{
  response_packet_t rep = {0};
  uint32_t MASK[4] = {0x00000007, 0, 0, 0};
  uint8_t secure_boot = 0, prov_done = 0;
  uint32_t NonSec_Dbg_Auth = HAL_BSEC_NONSEC_DBG_UNAUTH;

  HAL_StatusTypeDef status = HAL_ERROR;

  BSEC_HandleTypeDef sBsecHandler = {.Instance = BSEC};

  /* Get Dev ID */
  DiscoveryParameters.targetID = READ_REG(DBGMCU->IDCODE) & DBGMCU_IDCODE_DEV_ID;

  /* Get 96-bit unique ID located in OTP 5 - 7 */
  for (uint32_t i = 0; i < 3; i++)
  {
    if (HAL_BSEC_OTP_Read(&sBsecHandler, OTP_UID + i, &(DiscoveryParameters.socID[2 - i])) != HAL_OK)
    {
      return status;
    }
  }

  /* Update integ status */
  OTP_Get_Password_Hash(NULL);
  if (da_password_provisioned)
  {
    DiscoveryParameters.provIntegStatus = 0xeaeaeaea;
  }

  status = OTP_Get_Lifecycle_Status(&secure_boot, &prov_done);
  if (status != HAL_OK)
  {
    return status;
  }

  /* Close locked provd */
  if (secure_boot && prov_done)
  {
    DiscoveryParameters.lifeCycle[0] = 0x27;
    DiscoveryParameters.lifeCycle[1] = 0x30;
  }
  else if (secure_boot) /* Close locked unprovd */
  {
    DiscoveryParameters.lifeCycle[0] = 0x71;
    DiscoveryParameters.lifeCycle[1] = 0x20;
  }
  else /* Close unlocked unprovd */
  {
    DiscoveryParameters.lifeCycle[0] = 0xDE;
    DiscoveryParameters.lifeCycle[1] = 0x10;
  }
  status = Get_NonSec_Dbg_Auth(&NonSec_Dbg_Auth);
  if (status != HAL_OK)
  {
    return status;
  }

  if (NonSec_Dbg_Auth == HAL_BSEC_NONSEC_DBG_AUTH)
  {
    DiscoveryParameters.lifeCycle[1] = 0x50;
  }

  add_tlv(ADAC_AUTH_VERSION, (uint8_t*)DiscoveryParameters.psaAuthVer, sizeof(DiscoveryParameters.psaAuthVer));
  add_tlv(VENDOR_ID, (uint8_t*)DiscoveryParameters.vendorID, sizeof(DiscoveryParameters.vendorID));
  add_tlv(SOC_CLASS, (uint8_t*)&DiscoveryParameters.targetID, sizeof(DiscoveryParameters.targetID));
  add_tlv(SOC_ID, (uint8_t*)DiscoveryParameters.socID, sizeof(DiscoveryParameters.socID));
  add_tlv(HW_PERMISSIONS_MASK, (uint8_t*)MASK, sizeof(MASK));
  add_tlv(PSA_LIFECYCLE, (uint8_t*)DiscoveryParameters.lifeCycle, sizeof(DiscoveryParameters.lifeCycle));
  add_tlv(SDA_ID, (uint8_t*)&DiscoveryParameters.sdaVer, sizeof(DiscoveryParameters.sdaVer));
  add_tlv(TOKEN_FORMATS, (uint8_t*)&DiscoveryParameters.tokenFormats, sizeof(DiscoveryParameters.tokenFormats));
  add_tlv(CERT_FORMATS, (uint8_t*)&DiscoveryParameters.certifFormats, sizeof(DiscoveryParameters.certifFormats));
  add_tlv(CRYPTOSYSTEMS, (uint8_t*)&DiscoveryParameters.cryptosystems, sizeof(DiscoveryParameters.cryptosystems));
  add_tlv(PSA_ST_PASSWD, (uint8_t*)&DiscoveryParameters.provIntegStatus, sizeof(DiscoveryParameters.provIntegStatus));
  add_tlv(HDPL1_STATUS, (uint8_t*)&DiscoveryParameters.hdpl1Status, sizeof(DiscoveryParameters.hdpl1Status));
  add_tlv(HDPL2_STATUS, (uint8_t*)&DiscoveryParameters.hdpl2Status, sizeof(DiscoveryParameters.hdpl2Status));
  add_tlv(HDPL3_STATUS, (uint8_t*)&DiscoveryParameters.hdpl3Status, sizeof(DiscoveryParameters.hdpl3Status));

  rep.data_count = (byte_count + 5)/4;
  send_message((uint32_t*)&rep, 8, DA_TIMEOUT);

  return send_message((uint32_t*)buff, byte_count + 5, DA_TIMEOUT);
}

/**
 * @brief Add TLV (Type-Length-Value) Entry to Buffer
 *
 * This function adds a TLV (Type-Length-Value) entry to the specified buffer.
 * The TLV format is commonly used in communication protocols to encode data.
 *
 * @param[in] id             The type identifier for the TLV entry.
 * @param[in] buffer         Pointer to the data buffer.
 * @param[in] length_in_bytes The length of the value in bytes.
 */
static void add_tlv(uint16_t id, uint8_t *buffer, uint32_t length_in_bytes)
{
  psa_tlv_t psa_tlv = {0};

  psa_tlv.type_id = id;
  psa_tlv.length_in_bytes = length_in_bytes;
  memcpy(psa_tlv.value, buffer, length_in_bytes);
  memcpy(&buff[byte_count], &psa_tlv, 8 + length_in_bytes);
  byte_count += 8 + length_in_bytes;
}

/**
 * @brief Send a Message with Timeout
 *
 * This function sends a message of specified length and waits for the operation
 * to complete within a given timeout period.
 *
 * @param[in] message Pointer to the message to be sent.
 * @param[in] length  The length of the message in bytes.
 * @param[in] timeout The maximum time to wait for the message to be sent, in milliseconds.
 *
 * @return HAL_StatusTypeDef - status of the operation.
 */
static HAL_StatusTypeDef send_message(uint32_t *message, uint32_t length, uint32_t timeout)
{
  uint32_t word_count = length / sizeof(uint32_t);
  uint32_t word_offset = 0U;
  uint32_t tick_start = HAL_GetTick();

  while (word_offset < word_count)
  {
    /* Write a word */
    DBGMCU_DBG_AUTH_DEVICE_WRITE(message[word_offset++]);

    /* Wait reading of DBG_AUTH_DEV */
    while (DBGMCU_DBG_AUTH_DEVICE_READ_ACK() != 0U)
    {
      /* Wait data reading from HOST */
      if (HAL_GetTick() - tick_start > timeout)
      {
        return HAL_TIMEOUT;
      }
    }
  }

  return HAL_OK;
}

/**
 * @brief Receive a Message with Timeout
 *
 * This function receives a message of specified length and waits for the operation
 * to complete within a given timeout period.
 *
 * @param[out] buffer  Pointer to the buffer where the received message will be stored.
 * @param[in]  length  The length of the message to be received, in bytes.
 * @param[in]  timeout The maximum time to wait for the message to be received, in milliseconds.
 *
 * @return HAL_StatusTypeDef - status of the operation.
 */
static HAL_StatusTypeDef receive_message(uint32_t *buffer, uint32_t length, uint32_t timeout)
{
  uint32_t max_words = length / sizeof(uint32_t);
  uint32_t word_offset = 0U;
  HAL_StatusTypeDef status = HAL_ERROR;

  while (word_offset < max_words)
  {
    /* Read incoming word */
    uint32_t word = 0xFF;
    status = receive_word(&word, timeout);
    if (status != HAL_OK)
    {
      return status;
    }
    buffer[word_offset++] = word;
  }

  return HAL_OK;
}

/**
 * @brief Receive a Single Word with Timeout
 *
 * This function receives a single word (32-bit) and waits for the operation
 * to complete within a given timeout period.
 *
 * @param[out] word    Pointer to the variable where the received word will be stored.
 * @param[in]  timeout The maximum time to wait for the word to be received, in milliseconds.
 *
 * @return HAL_StatusTypeDef - status of the operation.
 */
static HAL_StatusTypeDef receive_word(uint32_t *word, uint32_t timeout)
{
  uint32_t tick_start = HAL_GetTick();

  *word = 0;

  /* Read Host ACK */
  while (DBGMCU_DBG_AUTH_HOST_READ_ACK() != 1U)
  {
    /* Wait data availability */
    if (HAL_GetTick() - tick_start > timeout)
    {
      return HAL_TIMEOUT;
    }
  }

  *word = DBGMCU_DBG_AUTH_HOST_READ();
  DBGMCU_DBG_AUTH_HOST_WRITE_ACK();

  return HAL_OK;
}

/**
 * @brief Write a Message to the Debug MCU Authentication Device Register
 * This function writes a 32-bit message to the DBG_AUTH_DEV register of the Debug MCU.
 * @param[in] message The 32-bit message to be written to the DBG_AUTH_DEV register.
 */
static void DBGMCU_DBG_AUTH_DEVICE_WRITE(uint32_t message)
{
  WRITE_REG(DBGMCU->DBG_AUTH_DEV, message);
}

/**
 * @brief Read a Message from the Debug MCU Authentication Host Register
 *
 * This function reads a 32-bit message from the DBG_AUTH_HOST register of the Debug MCU.
 * @return uint32_t The 32-bit message read from the DBG_AUTH_HOST register.
 */
static uint32_t DBGMCU_DBG_AUTH_HOST_READ(void)
{
  return READ_REG(DBGMCU->DBG_AUTH_HOST);
}

/**
 * @brief Read the Acknowledgment Status from the Debug MCU Authentication Device
 *
 * This function reads the acknowledgment status from the DBG_AUTH_ACK register to determine
 * if the device has acknowledged.
 * @return uint32_t
 *         - 1: Device has acknowledged
 *         - 0: Device has not acknowledged
 */
static uint32_t DBGMCU_DBG_AUTH_DEVICE_READ_ACK(void)
{
  return ((READ_BIT(DBGMCU->DBG_AUTH_ACK, DBGMCU_DBG_AUTH_ACK_DEVICE_ACK) == DBGMCU_DBG_AUTH_ACK_DEVICE_ACK) ? 1UL : 0UL);
}

/**
 * @brief Clear the Acknowledgment Status for the Debug MCU Authentication Host
 *
 * This function clears the host acknowledgment status in the DBG_AUTH_ACK register.
 */
static void DBGMCU_DBG_AUTH_HOST_WRITE_ACK(void)
{
  CLEAR_BIT(DBGMCU->DBG_AUTH_ACK, DBGMCU_DBG_AUTH_ACK_HOST_ACK);
}

/**
 * @brief Read the Acknowledgment Status from the Debug MCU Authentication Host
 *
 * This function reads the acknowledgment status from the DBG_AUTH_ACK register to determine
 * if the host has acknowledged.
 *
 * @return uint32_t
 *         - 1: Host has acknowledged
 *         - 0: Host has not acknowledged
 */
static uint32_t DBGMCU_DBG_AUTH_HOST_READ_ACK(void)
{
  return ((READ_BIT(DBGMCU->DBG_AUTH_ACK, DBGMCU_DBG_AUTH_ACK_HOST_ACK) == DBGMCU_DBG_AUTH_ACK_HOST_ACK) ? 1UL : 0UL);
}

/**
 * @brief Enable the Debug MCU
 *
 * This function enables the Debug MCU by setting the appropriate control bits.
 */
static void DBGMCU_DBG_ENABLE(void)
{
  LL_MISC_EnableClock(LL_DBG);
  __HAL_RCC_DBGMCU_CLK_ENABLE();
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBGCLKEN);
}
