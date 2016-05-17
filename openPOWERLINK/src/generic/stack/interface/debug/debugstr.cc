/**
 ********************************************************************************
 \file   debugstr.cc

 \brief  Debug String module

 This file implements the debug string module. It is used to convert a lot
 of openPOWERLINK enumerations and error codes into descriptive strings.

 \ingroup module_debugstr
 *******************************************************************************/

#include "debugstr.h"

using namespace std;
using namespace interface::api;

namespace interface
{
    namespace debug
    {

        typedef struct
        {
                NmtState nmtState;
                const char* sNmtState;
        } tNmtStateInfo;

        typedef struct
        {
                ApiEventType apiEvent;
                const char* sApiEvent;
        } tApiEventInfo;

        typedef struct
        {
                Error key;
                const char* sName;
        } tRetValInfo;

        typedef struct
        {
                UINT16 key;
                const char* sName;
        } tEmergErrCodeInfo;

        typedef struct
        {
                UINT32 abortCode;
                const char* sAbortCode;
        } tAbortCodeInfo;

        typedef struct
        {
                NmtNodeCommand nodeCommand;
                const char* sNodeCommand;
        } tNmtNodeCommandInfo;

        static const char* invalidStr_l = "INVALID";

// text strings for POWERLINK events
        static const char* nmtEventStr_l[] = { "NmtEventNoEvent",              //
                "NmtEventDllMePres",            //
                "NmtEventDllMePresTimeout",     //
                "NmtEventDllMeAsndTimeout",     //
                "NmtEventDllMeSoaSent",         //
                "NmtEventDllMeSocTrig",         //
                "NmtEventDllMeSoaTrig",         //
                "NmtEventDllCeSoc",             //
                "NmtEventDllCePreq",            //
                "NmtEventDllCePres",            //
                "NmtEventDllCeSoa",             //
                "NmtEventDllCeAInv",            //
                "NmtEventDllCeAsnd",            //
                "NmtEventDllCeFrameTimeout",    //
                "NmtEventDllReAmni",            //
                "NmtEventDllReSwitchOverTimeout", // reserved
                "NmtEventSwReset",              // NMT_GT1, NMT_GT2, NMT_GT8
                "NmtEventResetNode",            //
                "NmtEventResetCom",             //
                "NmtEventResetConfig",          //
                "NmtEventEnterPreOperational2", //
                "NmtEventEnableReadyToOperate", //
                "NmtEventStartNode",            // NMT_CT7
                "NmtEventStopNode",             //
                "NmtEventGoToStandby",          //
                "NmtEventGoToStandbyDelayed",   //
                "0x1A",                         // reserved
                "0x1B",                         // reserved
                "0x1C",                         // reserved
                "0x1D",                         // reserved
                "0x1E",                         // reserved
                "0x1F",                         // reserved
                "NmtEventEnterResetApp",        //
                "NmtEventEnterResetCom",        //
                "NmtEventInternComError",       // NMT_GT6, internal communication error -> enter ResetCommunication
                "NmtEventEnterResetConfig",     //
                "NmtEventEnterCsNotActive",     //
                "NmtEventEnterMsNotActive",     //
                "NmtEventTimerBasicEthernet",   // NMT_CT3; timer triggered state change (NotActive -> BasicEth)
                "NmtEventTimerMsPreOp1",        // enter PreOp1 on MN (NotActive -> MsPreOp1)
                "NmtEventNmtCycleError",        // NMT_CT11, NMT_MT6; error during cycle -> enter PreOp1
                "NmtEventTimerMsPreOp2",     // enter PreOp2 on MN (MsPreOp1 -> MsPreOp2 if NmtEventAllMandatoryCNIdent)
                "NmtEventAllMandatoryCNIdent",  // enter PreOp2 on MN if NmtEventTimerMsPreOp2
                "NmtEventEnterReadyToOperate",  // application ready for the state ReadyToOp
                "NmtEventEnterMsOperational",   // enter Operational on MN
                "NmtEventSwitchOff",            // enter state Off
                "NmtEventCriticalError",        // enter state Off because of critical error
                "NmtEventEnterRmsNotActive",    //
                };

// text strings for POWERLINK event sources
        static const char* eventSourceStr_l[] = { "0",                        // reserved

                // openPOWERLINK kernel modules
                "EventSourceDllk",// Dllk module
                "EventSourceNmtk",          // Nmtk module
                "EventSourceObdk",          // Obdk module
                "EventSourcePdok",          // Pdok module
                "EventSourceTimerk",        // Timerk module
                "EventSourceEventk",        // Eventk module
                "EventSourceSyncCb",        // sync-Cb
                "EventSourceErrk",          // Error handler module
                "0x09", "0x0a", "0x0b",     // reserved
                "0x0c", "0x0d", "0x0e",     // reserved
                "0x0f",                     // reserved

                // openPOWERLINK user modules
                "EventSourceDllu",// Dllu module
                "EventSourceNmtu",          // Nmtu module
                "EventSourceNmtCnu",        // NmtCnu module
                "EventSourceNmtMnu",        // NmtMnu module
                "EventSourceObdu",          // Obdu module
                "EventSourceSdoUdp",        // Sdo/Udp module
                "EventSourceSdoAsnd",       // Sdo/Asnd module
                "EventSourceSdoAsySeq",     // Sdo asynchronous Sequence Layer module
                "EventSourceSdoCom",        // Sdo command layer module
                "EventSourceTimeru",        // Timeru module
                "EventSourceCfgMau",        // CfgMau module
                "EventSourceEventu",        // Eventu module
                "EventSourceOplkApi",       // Api module
                "0x1d",                     // reserved
                "EventSourceGw309Ascii",    // GW309ASCII module
                "EventSourceErru"           // User error module
                };

// text strings for POWERLINK event sinks
        static const char* eventSinkStr_l[] = { "EventSinkSync", "EventSinkNmtk", "EventSinkDllk", "EventSinkDlluCal",
                "EventSinkDllkCal", "EventSinkPdok", "EventSinkNmtu", "EventSinkErrk", "EventSinkErru",
                "EventSinkSdoAsySeq", "EventSinkNmtMnu", "0x0b",                     // reserved
                "EventSinkPdokCal", "EventSinkGw309Ascii", "EventSinkApi" };

// text strings for POWERLINK event types
        static const char* eventTypeStr_l[] = { "0",                                // reserved
                "EventTypeNmtEvent",                // NMT event
                "EventTypePdoRx",                   // PDO frame received event (PRes/PReq)
                "EventTypePdoTx",                   // PDO frame transmitted event (PRes/PReq)
                "EventTypePdoSoa",                  // SoA frame received event (isochronous phase completed)
                "EventTypeSync",                    // Sync event (e.g. SoC or anticipated SoC)
                "EventTypeTimer",                   // Timer event
                "EventTypeHeartbeat",               // Heartbeat event
                "EventTypeHistoryEntry",            // Error history entry event
                "EventTypeDllkFlag1",               // DLL kernel Flag 1 changed event
                "EventTypeDllkFillTx",              // DLL kernel fill TxBuffer event
                "EventTypeDllkPresReady",           // DLL kernel PRes ready event
                "EventTypeError",                   // Error event for API layer
                "EventTypeNmtStateChange",          // indicate change of NMT-State
                "EventTypeDllError",                // DLL error event for Error handler
                "EventTypeAsndRx",                  // received ASnd frame for DLL user module
                "EventTypeDllkServFilter",          // configure ServiceIdFilter
                "EventTypeDllkIdentity",            // configure Identity
                "EventTypeDllkConfig",              // configure ConfigParam
                "EventTypeDllkIssueReq",            // issue Ident/Status request
                "EventTypeDllkAddNode",             // add node to isochronous phase
                "EventTypeDllkDelNode",             // remove node from isochronous phase
                "EventTypeDllkConfigNode",          // configures parameters of node
                "EventTypeDllkStartReducedCycle",   // start reduced POWERLINK cycle on MN
                "EventTypeNmtMnuNmtCmdSent",        // NMT command was actually sent
                "EventTypeApiUserDef",              // user-defined event
                "EventTypeDllkCycleFinish",         // SoA sent, cycle finished
                "0x1B",                             // reserved
                "0x1C",                             // reserved
                "0x1D",                             // reserved
                "0x1E",                             // reserved
                "0x1F",                             // reserved
                "EventTypePdokAlloc",               // alloc PDOs
                "EventTypePdokConfig",              // configure PDO channel
                "EventTypeNmtMnuNodeCmd",           // trigger NMT node command
                "EventTypeGw309AsciiReq",           // GW309ASCII request
                "EventTypeNmtMnuNodeAdded",         // node was added to isochronous phase by DLL
                "EventTypePdokSetupPdoBuf",         // dealloc PDOs
                "EventTypePdokControlSync",         // enable/disable the pdokcal sync trigger (arg is pointer to BOOL)
                "EventTypeReleaseRxFrame",          // Free receive buffer
                "EventTypeASndNotRx"                // Didn't receive ASnd frame for DLL user module
                };

// text strings for POWERLINK states
        static tNmtStateInfo nmtStateInfo_l[] = { { NmtStateE::kNmtGsOff, "NmtGsOff" }, { NmtStateE::kNmtGsInitialising,
                "NmtGsInitializing" }, { NmtStateE::kNmtGsResetApplication, "NmtGsResetApplication" }, {
                NmtStateE::kNmtGsResetCommunication, "NmtGsResetCommunication" }, { NmtStateE::kNmtGsResetConfiguration,
                "NmtGsResetConfiguration" }, { NmtStateE::kNmtCsNotActive, "NmtCsNotActive" }, {
                NmtStateE::kNmtCsPreOperational1, "NmtCsPreOperational1" },
                { NmtStateE::kNmtCsStopped, "NmtCsStopped" },
                { NmtStateE::kNmtCsPreOperational2, "NmtCsPreOperational2" }, { NmtStateE::kNmtCsReadyToOperate,
                        "NmtCsReadyToOperate" }, { NmtStateE::kNmtCsOperational, "NmtCsOperational" }, {
                        NmtStateE::kNmtCsBasicEthernet, "NmtCsBasicEthernet" }, { NmtStateE::kNmtMsNotActive,
                        "NmtMsNotActive" }, { NmtStateE::kNmtMsPreOperational1, "NmtMsPreOperational1" }, {
                        NmtStateE::kNmtMsPreOperational2, "NmtMsPreOperational2" }, { NmtStateE::kNmtMsReadyToOperate,
                        "NmtMsReadyToOperate" }, { NmtStateE::kNmtMsOperational, "NmtMsOperational" }, {
                        NmtStateE::kNmtMsBasicEthernet, "NmtMsBasicEthernet" }, { NmtStateE::kNmtRmsNotActive,
                        "NmtRmsNotActive" }, { NmtStateE::kNmtStateInvalid, "Invalid NMT State" }, };

// text strings for API events
        static tApiEventInfo apiEventInfo_l[] = { { ApiEvent::kOplkApiEventUserDef, "User defined" }, {
                ApiEvent::kOplkApiEventNmtStateChange, "NMT state change" }, { ApiEvent::kOplkApiEventCriticalError,
                "CRITICAL error -> stack halted" }, { ApiEvent::kOplkApiEventWarning, "Warning" }, {
                ApiEvent::kOplkApiEventHistoryEntry, "History entry" }, { ApiEvent::kOplkApiEventNode, "Node event" }, {
                ApiEvent::kOplkApiEventBoot, "Boot event" }, { ApiEvent::kOplkApiEventSdo, "SDO event" }, {
                ApiEvent::kOplkApiEventObdAccess, "OBD access" },
                { ApiEvent::kOplkApiEventCfmProgress, "CFM progress" },
                { ApiEvent::kOplkApiEventCfmResult, "CFM result" }, { ApiEvent::kOplkApiEventReceivedAsnd,
                        "Received ASnd" }, { ApiEvent::kOplkApiEventPdoChange, "PDO change" } };

// text strings for values of type tOplkError
        static tRetValInfo retValInfo_l[] =
                {
                /* area for generic errors 0x0000 - 0x000F */
                { Error::kErrorOk, "No error / function call successful" }, { Error::kErrorIllegalInstance,
                        "The called instance does not exist" }, { Error::kErrorInvalidInstanceParam,
                        "Invalid instance parameter" }, { Error::kErrorNoFreeInstance,
                        "Xxx_addInstance was called but no free instance is available" }, { Error::kErrorWrongSignature,
                        "Wrong signature while writing to object 0x1010 or 0x1011" }, { Error::kErrorInvalidOperation,
                        "Operation not allowed in this situation" }, { Error::kErrorInvalidNodeId,
                        "Invalid NodeId was specified" }, { Error::kErrorNoResource,
                        "Resource could not be created (Windows, PxROS, ...)" }, { Error::kErrorShutdown,
                        "Stack is shutting down" }, { Error::kErrorReject, "Reject the subsequent command" }, {
                        Error::kErrorRetry, "Retry this command" }, { Error::kErrorInvalidEvent,
                        "Invalid event was posted to process function" },
                        { Error::kErrorGeneralError, "General error" }, { Error::kErrorFeatureMismatch,
                                "Features of user and kernel stack are mismatched" },

                        /* area for EDRV module 0x0010 - 0x001F */
                        { Error::kErrorEdrvNoFreeTxDesc, "No free Tx descriptor available" }, {
                                Error::kErrorEdrvInvalidCycleLen, "Invalid cycle length (e.g. 0)" }, {
                                Error::kErrorEdrvInit, "Initialisation error" }, { Error::kErrorEdrvNoFreeBufEntry,
                                "No free entry in internal buffer table for Tx frames" }, {
                                Error::kErrorEdrvBufNotExisting, "Specified Tx buffer does not exist" }, {
                                Error::kErrorEdrvInvalidRxBuf, "Specified Rx buffer is invalid" }, {
                                Error::kErrorEdrvInvalidParam, "Invalid parameter in function call" }, {
                                Error::kErrorEdrvNextTxListNotEmpty,
                                "Next Tx buffer list is not empty, i.e. still in use" }, {
                                Error::kErrorEdrvCurTxListEmpty,
                                "Current Tx buffer list is empty, i.e. DLL didn't provide one" }, {
                                Error::kErrorEdrvTxListNotFinishedYet,
                                "Current Tx buffer list has not been finished yet, but new cycle has started" },

                        /* area for DLL module 0x0020 - 0x002F */
                        { Error::kErrorDllOutOfMemory, "Out of memory" }, { Error::kErrorDllIllegalHdl,
                                "Illegal handle for a TxFrame was passed" }, { Error::kErrorDllCbAsyncRegistered,
                                "Handler for non-POWERLINK frames had already been registered before" }, {
                                Error::kErrorDllAsyncSyncReqFull, "Buffer for SyncRequests is full" },
                        { Error::kErrorDllAsyncTxBufferEmpty, "Transmit buffer for asynchronous frames is empty" }, {
                                Error::kErrorDllAsyncTxBufferFull, "Transmit buffer for asynchronous frames is full" },
                        { Error::kErrorDllNoNodeInfo, "MN: Too less space in the internal node info structure" }, {
                                Error::kErrorDllInvalidParam, "Invalid parameters passed to function" }, {
                                Error::kErrorDllInvalidAsndServiceId, "Invalid AsndServiceId specified" }, {
                                Error::kErrorDllTxBufNotReady, "TxBuffer (e.g. for PReq) is not ready yet" },
                        { Error::kErrorDllTxFrameInvalid, "TxFrame (e.g. for PReq) is invalid or does not exist" },

                        /* area for OBD module 0x0030 - 0x003F */
                        { Error::kErrorObdIllegalPart, "Unknown OD part" }, { Error::kErrorObdIndexNotExist,
                                "Object index does not exist in the OD" }, { Error::kErrorObdSubindexNotExist,
                                "Subindex does not exist in the object index" }, { Error::kErrorObdReadViolation,
                                "Read access to a write-only object" }, { Error::kErrorObdWriteViolation,
                                "Write access to a read-only object" }, { Error::kErrorObdAccessViolation,
                                "Access not allowed" }, { Error::kErrorObdUnknownObjectType,
                                "Object type not defined or unknown" }, { Error::kErrorObdVarEntryNotExist,
                                "Object does not contain VarEntry structure" }, { Error::kErrorObdValueTooLow,
                                "Value to write to the object is too low" }, { Error::kErrorObdValueTooHigh,
                                "Value to write to the object is too high" }, { Error::kErrorObdValueLengthError,
                                "Value to write is too long or too short" }, { Error::kErrorObdErrnoSet,
                                "File I/O error occurred and errno is set" }, { Error::kErrorObdInvalidDcf,
                                "Device configuration file (CDC) is not valid" }, { Error::kErrorObdOutOfMemory,
                                "Out of memory" }, { Error::kErrorObdNoConfigData,
                                "No configuration data present (CDC is empty)" },

                        /* area for NMT module 0x0040 - 0x004F */
                        { Error::kErrorNmtUnknownCommand, "Unknown NMT command" }, {
                                Error::kErrorNmtInvalidFramePointer, "Pointer to the frame is not valid" }, {
                                Error::kErrorNmtInvalidEvent, "Invalid event sent to NMT module" }, {
                                Error::kErrorNmtInvalidState, "Unknown state in NMT state machine" }, {
                                Error::kErrorNmtInvalidParam, "Invalid parameters specified" }, {
                                Error::kErrorNmtSyncReqRejected, "SyncReq could not be issued" },

                        /* area for SDO/UDP module 0x0050 - 0x005F */
                        { Error::kErrorSdoUdpMissCb, "Missing callback-function pointer during init of module" }, {
                                Error::kErrorSdoUdpNoSocket, "Error during init of socket" }, {
                                Error::kErrorSdoUdpSocketError, "Error during usage of socket" }, {
                                Error::kErrorSdoUdpThreadError, "Error during start of listen thread" }, {
                                Error::kErrorSdoUdpNoFreeHandle, "No free connection handle for UDP" }, {
                                Error::kErrorSdoUdpSendError, "Error during sending the frame" }, {
                                Error::kErrorSdoUdpInvalidHdl, "The connection handle is invalid" },

                        /* area for SDO Sequence layer module 0x0060 - 0x006F */
                        { Error::kErrorSdoSeqMissCb, "No callback-function assigned" }, {
                                Error::kErrorSdoSeqNoFreeHandle, "No free handle for connection" }, {
                                Error::kErrorSdoSeqInvalidHdl, "Invalid handle in SDO sequence layer" }, {
                                Error::kErrorSdoSeqUnsupportedProt, "Unsupported protocol selected" }, {
                                Error::kErrorSdoSeqNoFreeHistory, "No free history entry" }, {
                                Error::kErrorSdoSeqFrameSizeError, "The size of the frames is not correct" }, {
                                Error::kErrorSdoSeqRequestAckNeeded,
                                "Indicates that the history buffer is full and an ACK request is needed" }, {
                                Error::kErrorSdoSeqInvalidFrame, "Frame not valid" }, {
                                Error::kErrorSdoSeqConnectionBusy, "Connection is busy -> retry later" }, {
                                Error::kErrorSdoSeqInvalidEvent, "Invalid event received" },

                        /* area for SDO Command Layer module 0x0070 - 0x007F */
                        { Error::kErrorSdoComUnsupportedProt, "Unsupported Protocol selected" }, {
                                Error::kErrorSdoComNoFreeHandle, "No free handle for connection" }, {
                                Error::kErrorSdoComInvalidServiceType, "Invalid SDO service type specified" }, {
                                Error::kErrorSdoComInvalidHandle, "Handle invalid" }, {
                                Error::kErrorSdoComInvalidSendType, "The passed frame type is invalid for sending" },
                        { Error::kErrorSdoComNotResponsible,
                                "Internal error: command layer handle is not responsible for this event from sequence layer" },
                        { Error::kErrorSdoComHandleExists, "Handle to same node already exists" }, {
                                Error::kErrorSdoComHandleBusy, "Transfer via this handle is already running" }, {
                                Error::kErrorSdoComInvalidParam, "Invalid parameters passed to function" },

                        /* area for openPOWERLINK event module 0x0080 - 0x008F */
                        { Error::kErrorEventUnknownSink, "Unknown sink for event" }, { Error::kErrorEventPostError,
                                "Error during posting the event" }, { Error::kErrorEventReadError,
                                "Error during reading the event from queue" }, { Error::kErrorEventWrongSize,
                                "Event arg has wrong size" },

                        /* area for openPOWERLINK timer module 0x0090 - 0x009F */
                        { Error::kErrorTimerInvalidHandle, "Invalid handle for timer" }, {
                                Error::kErrorTimerNoTimerCreated, "No timer was created because of an error" }, {
                                Error::kErrorTimerThreadError, "Process thread could not be created" },

                        /* area for openPOWERLINK SDO/Asnd module 0x00A0 - 0x0AF */
                        { Error::kErrorSdoAsndInvalidNodeId, "Node id is invalid" }, { Error::kErrorSdoAsndNoFreeHandle,
                                "No free handle for connection" }, { Error::kErrorSdoAsndInvalidHandle,
                                "Handle for connection is invalid" },

                        /* area for PDO module 0x00B0 - 0x00BF  */
                        { Error::kErrorPdoNotExist, "Selected PDO does not exist" }, { Error::kErrorPdoLengthExceeded,
                                "Length of PDO mapping exceeds the current payload limit" }, {
                                Error::kErrorPdoGranularityMismatch,
                                "Configured PDO granularity is not equal to supported granularity" }, {
                                Error::kErrorPdoInitError, "Error during initialisation of PDO module" }, {
                                Error::kErrorPdoConfWhileEnabled,
                                "PDO configuration cannot be changed while it is enabled" }, {
                                Error::kErrorPdoErrorMapp, "Invalid PDO mapping" }, { Error::kErrorPdoVarNotFound,
                                "The referenced object in a PDO mapping does not exist" },
                        { Error::kErrorPdoVarNotMappable, "The referenced object in a PDO mapping is not mappable" }, {
                                Error::kErrorPdoSizeMismatch,
                                "Bit size of object mapping is larger than the object size" }, {
                                Error::kErrorPdoTooManyTxPdos,
                                "Too many TPDOs are existing (only one TPDO is allowed on a CN)" }, {
                                Error::kErrorPdoInvalidObjIndex,
                                "Invalid object index used for PDO mapping or communication parameter" }, {
                                Error::kErrorPdoTooManyPdos, "Too many PDOs are existing" },

                        /* Configuration manager module 0x00C0 - 0x00CF */
                        { Error::kErrorCfmConfigError, "Error in configuration manager" },
                        { Error::kErrorCfmSdocTimeOutError, "Error in configuration manager due to an SDO timeout" }, {
                                Error::kErrorCfmInvalidDcf, "Device configuration file (CDC) is not valid" }, {
                                Error::kErrorCfmUnsupportedDcf, "Unsupported DCF format" }, {
                                Error::kErrorCfmConfigWithErrors, "Configuration finished with errors" }, {
                                Error::kErrorCfmNoFreeConfig, "No free configuration entry" }, {
                                Error::kErrorCfmNoConfigData, "No configuration data present" }, {
                                Error::kErrorCfmUnsuppDatatypeDcf,
                                "Unsupported datatype found in DCF -> this entry was not configured" },

                        /* area for OD configuration store restore module 0x0D0 - 0x0DF */
                        { Error::kErrorObdStoreHwError, "HW error while accessing non-volatile memory" }, {
                                Error::kErrorObdStoreInvalidState,
                                "Non-volatile memory is in invalid state (nothing saved)" }, {
                                Error::kErrorObdStoreDataLimitExceeded, "Data count is less than the expected size" }, {
                                Error::kErrorObdStoreDataObsolete, "Data stored in the archive is obsolete" },

                        { Error::kErrorApiTaskDeferred,
                                "openPOWERLINK performs a task in the background and informs the application (or vice-versa) when it is finished" },
                        { Error::kErrorApiInvalidParam,
                                "Invalid parameters were passed to a function (e.g. invalid node id)" }, {
                                Error::kErrorApiNoObdInitRam, "No function pointer for ObdInitRam supplied" },
                        { Error::kErrorApiSdoBusyIntern,
                                "The SDO channel to this node is internally used by the stack (e.g. the CFM) and currently not available for the application." },
                        { Error::kErrorApiPIAlreadyAllocated, "Process image is already allocated" }, {
                                Error::kErrorApiPIOutOfMemory, "Process image: out of memory" }, {
                                Error::kErrorApiPISizeExceeded,
                                "Process image: variable linking or copy job exceeds the size of the PI" }, {
                                Error::kErrorApiPINotAllocated, "Process image is not allocated" }, {
                                Error::kErrorApiPIJobQueueFull, "Process image: job queue is full" }, {
                                Error::kErrorApiPIJobQueueEmpty, "Process image: job queue is empty" }, {
                                Error::kErrorApiPIInvalidJobSize, "Process image: invalid job size" }, {
                                Error::kErrorApiPIInvalidPIPointer,
                                "Process image: pointer to application's process image is invalid" }, {
                                Error::kErrorApiPINonBlockingNotSupp,
                                "Process image: non-blocking copy jobs are not supported on this target" }, {
                                Error::kErrorApiNotInitialized, "API called but stack is not initialized/running" }, };

        static const tEmergErrCodeInfo emergErrCodeInfo_l[] = { { E_NO_ERROR, "E_NO_ERROR" },

        // 0xFxxx manufacturer specific error codes
                { E_NMT_NO_IDENT_RES, "E_NMT_NO_IDENT_RES" }, { E_NMT_NO_STATUS_RES, "E_NMT_NO_STATUS_RES" },

                // 0x816x HW errors
                { E_DLL_BAD_PHYS_MODE, "E_DLL_BAD_PHYS_MODE" }, { E_DLL_COLLISION, "E_DLL_COLLISION" }, {
                        E_DLL_COLLISION_TH, "E_DLL_COLLISION_TH" }, { E_DLL_CRC_TH, "E_DLL_CRC_TH " }, {
                        E_DLL_LOSS_OF_LINK, "E_DLL_LOSS_OF_LINK", }, { E_DLL_MAC_BUFFER, "E_DLL_MAC_BUFFER", },

                // 0x82xx Protocol errors
                { E_DLL_ADDRESS_CONFLICT, "E_DLL_ADDRESS_CONFLICT" }, { E_DLL_MULTIPLE_MN, "E_DLL_MULTIPLE_MN" },

                // 0x821x Frame size errors
                { E_PDO_SHORT_RX, "E_PDO_SHORT_RX" }, { E_PDO_MAP_VERS, "E_PDO_MAP_VERS" }, { E_NMT_ASND_MTU_DIF,
                        "E_NMT_ASND_MTU_DIF" }, { E_NMT_ASND_MTU_LIM, "E_NMT_ASND_MTU_LIM" }, { E_NMT_ASND_TX_LIM,
                        "E_NMT_ASND_TX_LIM" },

                // 0x823x Timing errors
                { E_NMT_CYCLE_LEN, "E_NMT_CYCLE_LEN" }, { E_DLL_CYCLE_EXCEED, "E_DLL_CYCLE_EXCEED" }, {
                        E_DLL_CYCLE_EXCEED_TH, "E_DLL_CYCLE_EXCEED_TH" }, { E_NMT_IDLE_LIM, "E_NMT_IDLE_LIM" }, {
                        E_DLL_JITTER_TH, "E_DLL_JITTER_TH" }, { E_DLL_LATE_PRES_TH, "E_DLL_LATE_PRES_TH" }, {
                        E_NMT_PREQ_CN, "E_NMT_PREQ_CN" }, { E_NMT_PREQ_LIM, "E_NMT_PREQ_LIM" }, { E_NMT_PRES_CN,
                        "E_NMT_PRES_CN" }, { E_NMT_PRES_RX_LIM, "E_NMT_PRES_RX_LIM" }, {
                E_NMT_PRES_TX_LIM, "E_NMT_PRES_TX_LIM" },

                // 0x824x Frame errors
                { E_DLL_INVALID_FORMAT, "E_DLL_INVALID_FORMAT" }, { E_DLL_LOSS_PREQ_TH, "E_DLL_LOSS_PREQ_TH" }, {
                E_DLL_LOSS_PRES_TH, "E_DLL_LOSS_PRES_TH" }, { E_DLL_LOSS_SOA_TH, "E_DLL_LOSS_SOA_TH" }, {
                E_DLL_LOSS_SOC_TH, "E_DLL_LOSS_SOC_TH" },

                // 0x84xx BootUp Errors
                { E_NMT_BA1, "E_NMT_BA1" }, { E_NMT_BA1_NO_MN_SUPPORT, "E_NMT_BA1_NO_MN_SUPPORT" }, { E_NMT_BPO1,
                        "E_NMT_BPO1" }, { E_NMT_BPO1_GET_IDENT, "E_NMT_BPO1_GET_IDENT" }, { E_NMT_BPO1_DEVICE_TYPE,
                        "E_NMT_BPO1_DEVICE_TYPE" }, { E_NMT_BPO1_VENDOR_ID, "E_NMT_BPO1_VENDOR_ID" }, {
                        E_NMT_BPO1_PRODUCT_CODE, "E_NMT_BPO1_PRODUCT_CODE" }, { E_NMT_BPO1_REVISION_NO,
                        "E_NMT_BPO1_REVISION_NO" }, {
                E_NMT_BPO1_SERIAL_NO, "E_NMT_BPO1_SERIAL_NO" }, { E_NMT_BPO1_CF_VERIFY, "E_NMT_BPO1_CF_VERIFY" }, {
                E_NMT_BPO2, "E_NMT_BPO2" }, { E_NMT_BRO, "E_NMT_BRO" }, { E_NMT_WRONG_STATE, "E_NMT_WRONG_STATE" }, };

// text strings for NMT node events
        static const char* OplkNmtNodeEvtTypeStr_g[] = { "Found",                    // 0x00
                "Update software",          // 0x01
                "Check configuration",      // 0x02
                "Update configuration",     // 0x03
                "Verify configuration",     // 0x04
                "Ready to start",           // 0x05
                "NMT state",                // 0x06
                "NMT error",                // 0x07
                };

// text strings for NMT boot events
        static const char* OplkNmtBootEvtTypeStr_g[] = { "Boot step 1 finished",     // 0x00     PreOp2 is possible
                "Boot step 2 finished",     // 0x01     ReadyToOp is possible for MN
                "EnableReadyToOp",          // 0x02     ReadyToOP is possible for CN
                "CheckComFinish",           // 0x03     Operational is possible
                "Operational",              // 0x04     all mandatory CNs are Operational
                "Error",                    // 0x05
                };

// text strings for SDO command layer connection states
        static const char* OplkSdoComConStateStr_g[] = { "NotActive",                // 0x00
                "Running",                  // 0x01
                "TxAborted",                // 0x02
                "RxAborted",                // 0x03
                "Finished",                 // 0x04
                "LowerLayerAbort",          // 0x05
                };

// text strings for abort codes
        static tAbortCodeInfo abortCodeInfo_g[] = { { SDO_AC_TIME_OUT, "SDO_AC_TIME_OUT" }, {
                SDO_AC_UNKNOWN_COMMAND_SPECIFIER, "SDO_AC_UNKNOWN_COMMAND_SPECIFIER" }, { SDO_AC_INVALID_BLOCK_SIZE,
                "SDO_AC_INVALID_BLOCK_SIZE" }, {
        SDO_AC_INVALID_SEQUENCE_NUMBER, "SDO_AC_INVALID_SEQUENCE_NUMBER" }, { SDO_AC_OUT_OF_MEMORY,
                "SDO_AC_OUT_OF_MEMORY" }, { SDO_AC_UNSUPPORTED_ACCESS, "SDO_AC_UNSUPPORTED_ACCESS" }, {
        SDO_AC_READ_TO_WRITE_ONLY_OBJ, "SDO_AC_READ_TO_WRITE_ONLY_OBJ" }, { SDO_AC_WRITE_TO_READ_ONLY_OBJ,
                "SDO_AC_WRITE_TO_READ_ONLY_OBJ" }, { SDO_AC_OBJECT_NOT_EXIST, "SDO_AC_OBJECT_NOT_EXIST" }, {
        SDO_AC_OBJECT_NOT_MAPPABLE, "SDO_AC_OBJECT_NOT_MAPPABLE" }, { SDO_AC_PDO_LENGTH_EXCEEDED,
                "SDO_AC_PDO_LENGTH_EXCEEDED" },
                { SDO_AC_GEN_PARAM_INCOMPATIBILITY, "SDO_AC_GEN_PARAM_INCOMPATIBILITY" }, {
                SDO_AC_INVALID_HEARTBEAT_DEC, "SDO_AC_INVALID_HEARTBEAT_DEC" }, { SDO_AC_GEN_INTERNAL_INCOMPATIBILITY,
                        "SDO_AC_GEN_INTERNAL_INCOMPATIBILITY" }, { SDO_AC_ACCESS_FAILED_DUE_HW_ERROR,
                        "SDO_AC_ACCESS_FAILED_DUE_HW_ERROR" }, { SDO_AC_DATA_TYPE_LENGTH_NOT_MATCH,
                        "SDO_AC_DATA_TYPE_LENGTH_NOT_MATCH" }, { SDO_AC_DATA_TYPE_LENGTH_TOO_HIGH,
                        "SDO_AC_DATA_TYPE_LENGTH_TOO_HIGH" }, { SDO_AC_DATA_TYPE_LENGTH_TOO_LOW,
                        "SDO_AC_DATA_TYPE_LENGTH_TOO_LOW" },
                { SDO_AC_SUB_INDEX_NOT_EXIST, "SDO_AC_SUB_INDEX_NOT_EXIST" }, {
                SDO_AC_VALUE_RANGE_EXCEEDED, "SDO_AC_VALUE_RANGE_EXCEEDED" }, { SDO_AC_VALUE_RANGE_TOO_HIGH,
                        "SDO_AC_VALUE_RANGE_TOO_HIGH" }, { SDO_AC_VALUE_RANGE_TOO_LOW, "SDO_AC_VALUE_RANGE_TOO_LOW" }, {
                SDO_AC_MAX_VALUE_LESS_MIN_VALUE, "SDO_AC_MAX_VALUE_LESS_MIN_VALUE" }, { SDO_AC_GENERAL_ERROR,
                        "SDO_AC_GENERAL_ERROR" },
                { SDO_AC_DATA_NOT_TRANSF_OR_STORED, "SDO_AC_DATA_NOT_TRANSF_OR_STORED" }, {
                SDO_AC_DATA_NOT_TRANSF_DUE_LOCAL_CONTROL, "SDO_AC_DATA_NOT_TRANSF_DUE_LOCAL_CONTROL" }, {
                SDO_AC_DATA_NOT_TRANSF_DUE_DEVICE_STATE, "SDO_AC_DATA_NOT_TRANSF_DUE_DEVICE_STATE" }, {
                SDO_AC_OBJECT_DICTIONARY_NOT_EXIST, "SDO_AC_OBJECT_DICTIONARY_NOT_EXIST" }, { SDO_AC_CONFIG_DATA_EMPTY,
                        "SDO_AC_CONFIG_DATA_EMPTY" }, { 0, "SDO_AC_OK" } };

        static tNmtNodeCommandInfo nmtNodeCommandInfo_l[] = { { NmtNodeCommandE::kNmtNodeCommandBoot,
                "NmtNodeCommandBoot" }, { NmtNodeCommandE::kNmtNodeCommandSwOk, "NmtNodeCommandSwOk" }, {
                NmtNodeCommandE::kNmtNodeCommandSwUpdated, "NmtNodeCommandSwUpdated" }, {
                NmtNodeCommandE::kNmtNodeCommandConfOk, "NmtNodeCommandConfOk" }, {
                NmtNodeCommandE::kNmtNodeCommandConfRestored, "NmtNodeCommandConfRestored" }, {
                NmtNodeCommandE::kNmtNodeCommandConfReset, "NmtNodeCommandConfReset" }, {
                NmtNodeCommandE::kNmtNodeCommandConfErr, "NmtNodeCommandConfErr" }, {
                NmtNodeCommandE::kNmtNodeCommandStart, "NmtNodeCommandStart" }, };

//============================================================================//
//            P U B L I C   F U N C T I O N S                                 //
//============================================================================//

#ifndef tabentries
#define tabentries(aVar_p)  (sizeof(aVar_p) / sizeof(*(aVar_p)))
#endif

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified event

         The function returns the string describing the specified event.

         \param  nmtEvent_p          Event to print

         \return The function returns a string describing the specified event.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getNmtEventStr(NmtEvent nmtEvent)
        {
            if (nmtEvent >= tabentries(nmtEventStr_l))
            {
                return invalidStr_l;
            }
            else
            {
                return nmtEventStr_l[nmtEvent];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified event source

         The function returns the string describing the specified event source.

         \param  eventSrc_p          Event source to print

         \return The function returns a string describing the specified event source.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getEventSourceStr(EventSource eventSrc)
        {
            if (eventSrc >= tabentries(eventSourceStr_l))
            {
                return invalidStr_l;
            }
            else
            {
                return eventSourceStr_l[eventSrc];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified event sink

         The function returns the string describing the specified event sink.

         \param  eventSink_p         Event sink to print

         \return The function returns a string describing the specified event sink.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getEventSinkStr(EventSink eventSink)
        {
            if (eventSink >= tabentries(eventSinkStr_l))
            {
                return invalidStr_l;
            }
            else
            {
                return eventSinkStr_l[eventSink];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified event type

         The function returns the string describing the specified event type.

         \param  eventType_p         Event type to print

         \return The function returns a string describing the specified event type.

         \ingroup module_debug
         */
//------------------------------------------------------------------------------
        const char* getEventTypeStr(EventType eventType)
        {
            if (eventType >= tabentries(eventTypeStr_l))
            {
                return invalidStr_l;
            }
            else
            {
                return eventTypeStr_l[eventType];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified NMT state

         The function returns the string describing the specified NMT state.

         \param  nmtState_p         NMT state to print

         \return The function returns a string describing the specified NMT state.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getNmtStateStr(NmtState nmtState)
        {
            unsigned int i;

            for (i = 0; i < tabentries(nmtStateInfo_l); i++)
            {
                if (nmtStateInfo_l[i].nmtState == nmtState)
                    return (nmtStateInfo_l[i].sNmtState);
            }
            return invalidStr_l;
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified API event

         The function returns the string describing the specified API event.

         \param  ApiEvent_p         API event to print

         \return The function returns a string describing the specified API event.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getApiEventStr(ApiEventType ApiEvent)
        {
            UINT i;

            for (i = 0; i < tabentries(apiEventInfo_l); i++)
            {
                if (apiEventInfo_l[i].apiEvent == ApiEvent)
                    return (apiEventInfo_l[i].sApiEvent);
            }
            return invalidStr_l;
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified NMT node event

         The function returns the string describing the specified NMT node event.

         \param  NodeEventType_p         NMT node event to print

         \return The function returns a string describing the specified NMT node event.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getNmtNodeEventTypeStr(NmtNodeEvent NodeEventType)
        {
            if (NodeEventType >= tabentries(OplkNmtNodeEvtTypeStr_g))
            {
                return invalidStr_l;
            }
            else
            {
                return OplkNmtNodeEvtTypeStr_g[NodeEventType];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified NMT boot event

         The function returns the string describing the specified NMT boot event.

         \param  BootEventType_p         NMT boot event to print

         \return The function returns a string describing the specified NMT boot event.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getNmtBootEventTypeStr(NmtBootEvent BootEventType)
        {
            if (BootEventType >= tabentries(OplkNmtBootEvtTypeStr_g))
            {
                return invalidStr_l;
            }
            else
            {
                return OplkNmtBootEvtTypeStr_g[BootEventType];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified NMT node command

         The function returns the string describing the specified NMT node command.

         \param  nodeCommand_p         NMT node command to print

         \return The function returns a string describing the specified NMT node command.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getNmtNodeCommandTypeStr(NmtNodeCommand nodeCommand)
        {
            UINT i;

            for (i = 0; i < tabentries(nmtNodeCommandInfo_l); i++)
            {
                if (nmtNodeCommandInfo_l[i].nodeCommand == nodeCommand)
                    return (nmtNodeCommandInfo_l[i].sNodeCommand);
            }
            return invalidStr_l;
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified SDO command connection state

         The function returns the string describing the specified SDO command connection
         state.

         \param  SdoComConState_p         SDO command connection state to print

         \return The function returns a string describing the specified SDO command
         connection state.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getSdoComConStateStr(SdoComConState SdoComConState)
        {
            if (SdoComConState >= tabentries(OplkSdoComConStateStr_g))
            {
                return invalidStr_l;
            }
            else
            {
                return OplkSdoComConStateStr_g[SdoComConState];
            }
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string of the specified SDO command connection state

         The function returns the string describing the given entry of type tOplkError.

         \param  OplkError_p         tOplkError value to print

         \return The function returns a string describing the specified tOplkError type.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getRetValStr(ErrorType OplkError)
        {
            UINT i;

            for (i = 0; i < tabentries(retValInfo_l); i++)
            {
                if (retValInfo_l[i].key == OplkError)
                    return (retValInfo_l[i].sName);
            }
            return invalidStr_l;
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string describing the specified emergency error code

         The function returns the string describing the specified emergency error code.

         \param  emergErrCode_p       Emergency error code value to print

         \return The function returns a string describing the specified emergency error
         code.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getEmergErrCodeStr(UINT16 emergErrCode)
        {

            UINT i;

            for (i = 0; i < tabentries(emergErrCodeInfo_l); i++)
            {
                if (emergErrCodeInfo_l[i].key == emergErrCode)
                    return (emergErrCodeInfo_l[i].sName);
            }
            return invalidStr_l;
        }

//------------------------------------------------------------------------------
        /**
         \brief  Return the string describing the specified abort code

         The function returns the string describing the specified abort code.

         \param  abortCode_p       Abort code value to print

         \return The function returns a string describing the specified abort code.

         \ingroup module_debugstr
         */
//------------------------------------------------------------------------------
        const char* getAbortCodeStr(UINT32 abortCode)
        {
            tAbortCodeInfo* pEntry;
            UINT i;

            pEntry = abortCodeInfo_g;
            for (i = 0; i < tabentries(abortCodeInfo_g); i++)
            {
                if (pEntry->abortCode == abortCode)
                {
                    return pEntry->sAbortCode;
                }
                pEntry++;
            }
            return invalidStr_l;
        }
    }
}
