#include <bits/stdc++.h>
#include <iostream>
#include "common/include/setup.h"
#include "platform/include/platform.h"
#include "platform/include/logging.h"
#include "ngap/include/NGAP-PDU.h"
#include "ngap/include/InitialUEMessage.h"
#include "ngap/include/NGSetupRequest.h"
#include "ngap/include/InitiatingMessage.h"
#include "nas/NasMessage.h"
#include "nas/mobility/AuthenticationRequest.h"
#include "nas/mobility/AuthenticationReject.h"
#include "nas/mobility/AuthenticationResponse.h"
#include "nas/mobility/SecurityModeCommand.h"
#include "nas/mobility/RegistrationComplete.h"
#include "nas/mobility/RegistrationReject.h"
#include "nas/mobility/nasFields.h"
#include "nas/mobility/RegistrationRequest.h"
#include "ngap/include/constr_TYPE.h"
#include "ngap/include/xer_encoder.h"
#include <stdlib.h>
#include <unistd.h>
#include "ngap/codec/include/ngap_helper.h"
#include "test_ran/include/test_ue.h"
#include "common/include/aka.h"
#include "aka/include/securityKeysUtil.h"
#include "aka/include/milenage.h"
#include "common/include/utils.h"
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>

using namespace std;

#define THREADS 100000

atomic<bool> UEflag(true);

unsigned long long int No_Of_Req=0;

mutex mtx;
int a[THREADS][2];



#define MCC_HUNDREDS(vALUE) ((vALUE) / 100)
/* When MNC is only composed of 2 digits, set the hundreds unit to 0xf */
#define MNC_HUNDREDS(vALUE, mNCdIGITlENGTH) ( mNCdIGITlENGTH == 2 ? 15 : (vALUE) / 100)
#define MCC_MNC_DECIMAL(vALUE) (((vALUE) / 10) % 10)
#define MCC_MNC_DIGIT(vALUE) ((vALUE) % 10)

/* Convert an integer on 16 bits to the given bUFFER */
#define INT16_TO_BUFFER(x, buf) \
do {                            \
	(buf)[0] = (x) >> 8;        \
	(buf)[1] = (x);             \
} while(0)

#define INT16_TO_OCTET_STRING(x, aSN)           \
do {                                            \
	(aSN)->buf = (uint8_t *)calloc(2, sizeof(uint8_t));    \
	(aSN)->size = 2;              \
	INT16_TO_BUFFER(x, (aSN)->buf);             \
} while(0)


void printHex(uint8_t* inputArray, size_t size)
{
	for(uint i=0;i<size;i++)
	{
		//print each byte as hex
		printf("%02X ",(int)inputArray[i]);
	}
	printf("\n");
}
//Temporary functions++, TODO:fetch values from config file
int getLongTermKey(supi_t suci, uint8_t (&KA)[KA_SIZE])
{
	//int rc = FAILURE;
	uint8_t temp[KA_SIZE]={0x34, 0xb7, 0xd5, 0xee, 0xe1, 0x1a, 0x21, 0x62,0xbb, 0x20, 0xf3, 0xc4, 0xe5, 0x77, 0x36, 0x74};
	bytecopy(KA, temp, KA_SIZE);
	return SUCCESS;//TODO manage rc if successful to get value from config file
}

int getOperatorArray(supi_t suci, uint8_t (&operator_arr)[OP_SIZE])
{
	//int rc = FAILURE;
	uint8_t temp[OP_SIZE] ={0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65, 0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};
	bytecopy(operator_arr, temp, OP_SIZE);
	return SUCCESS;//TODO manage rc depending successful to get value from config file
}
//--

void ranHandleForAmf(void *msgBlob, int len, int streamId)
{
	//(amf::getInstance())->entryPoint(msgBlob, E_LOAD_GENERATOR_INST_1);
	free(msgBlob);
}

void registerCallbacks(struct nfvInstanceData* nfvInst)
{
	nfvInst->CB[E_LOAD_GENERATOR_INST_1] = 0;
	nfvInst->CB[E_AMF_INST_1] = ranHandleForAmf;
}

struct nfvInstanceData* init(_e_nfv_component comp)
{
	struct nfvInstanceData *nfvInst;

	nfvInst = initPlatform(comp);
	registerCallbacks(nfvInst);
	return nfvInst;
}




/*  TODO send valid values, free calloc'ed structs  */
void testSendInitialUeMessage(struct fdData * fdd, testUeContext_t testUe)
{
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	int rc;
	size_t errlen = MAX_MESSAGE_SIZE;
	asn_enc_rval_t encRet;
	NGAP_PDU_t  *ngapPdu = 0;
	InitiatingMessage_t  *initiatingMsg = 0;
	InitialUEMessage_t  *initUEmsg = 0;
	/* the IEs we will be  filling */
	ProtocolIE_Field_t *ranUeNgapId_IE = 0;
	ProtocolIE_Field_t *nasPdu_IE = 0;
	char test_nas_pdu[MAX_MESSAGE_SIZE] = {};
	uint32_t test_nas_pdu_len = 0;

	/*an elaborate field, commented out for now */
	//ProtocolIE_Field_t *userLocationInformation_IE = 0;
	//UserLocationInformation_t *usrLocInfo = 0;
	//UserLocationInformationNR_t *usrLocInfoNR = 0;
	//char test_plmnid[] = "1234";
	//char test_tac[] = "567";
	//uint8_t test_bits[] = {0xF1, 0x20}; /* = FF 2, 12 bits last 4 bits empty*/
	//int test_bits_len = 2;
	//int test_bits_num = 12;

	//TODO free calloc'ed data on failure SEQ FREE
	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	/* choose initiatingMessage */
	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	/* it is a initial UE msg */
	initiatingMsg->procedureCode = 15; /* asn_VAL_29_id_InitialUEMessage */
	initiatingMsg->value.present =
				InitiatingMessage__value_PR_InitialUEMessage;
	initUEmsg = &initiatingMsg->value.choice.InitialUEMessage;

	/* start filling following fields */

	/* 1. RAN-UE-NGAP-ID */
	ranUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(ranUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	ranUeNgapId_IE->id = 91;              /* asn_VAL_1_id_RAN_UE_NGAP_ID */
	ranUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_RAN_UE_NGAP_ID;
	ranUeNgapId_IE->value.choice.RAN_UE_NGAP_ID = testUe.ranUeNgapId;
	rc = ASN_SEQUENCE_ADD(&initUEmsg->protocolIEs.list, ranUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* 2. NAS-PDU */
	nasPdu_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(nasPdu_IE == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	nasPdu_IE->id = 40;                      /* asn_VAL_4_id_NAS_PDU */
	nasPdu_IE->value.present = ProtocolIE_Field__value_PR_NAS_PDU;
		/* 2.1 fill up NAS PDU*/
	
	/* Create a NAS - Registration Request Messaeg usuing new NAS  */
	nasMessage_t	nasMsg;	
	RegistrationRequest_t *regReqMsg = &(nasMsg.plain._5gmmMsg.regReqMsg);
	regReqMsg->mmHeader.epd 			= _5GS_MOBILITY_MANAGEMENT_MESSAGE;
	regReqMsg->mmHeader.secuHeader 	= SECURITY_HEADER_TYPE_NOT_PROTECTED;
	regReqMsg->mmHeader.msgType 		= REGISTRATION_REQUEST;
	regReqMsg->ngKsi.tsc 	= 0;
	regReqMsg->ngKsi.ngksi	= 0x07;

	// srand(time(0));
	
	regReqMsg->_5gmobileId.len = REG_REQ_5G_MOBILE_ID_LEN -
                                 REG_REQ_5G_MOBILE_ID_IEI_OFFSET;
    /* remove IEI offset since it is a mandatory field
     * FIXME: clean up the len var and make it easier to fill it up */
	regReqMsg->_5gmobileId.suci5gMobileId.supiFormat = E_IMSI;
	regReqMsg->_5gmobileId.suci5gMobileId.oddEven = _5G_MOBILE_IDENTITY_EVEN;
	regReqMsg->_5gmobileId.suci5gMobileId.identityType = E_SUCI;
	regReqMsg->_5gmobileId.suci5gMobileId.protectionSchId = E_NULL_SCHEME;
	regReqMsg->_5gmobileId.suci5gMobileId.homeNtwrkPKI = 0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput1 = testUe.ranUeNgapId;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput2 = testUe.ranUeNgapId;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput3 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput4 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput5 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput6 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput7 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput8 = 0x4;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput9 = 0x4;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput10 = testUe.ranUeNgapId;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput11 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput12 = 0x6;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput13 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput14 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput15 = 0x2;
	//TODO: SUPI should be 453276
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit1 = testUe.ranUeNgapId; 
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit2 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit3 = 0x3;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit1 = 0x1;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit2 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit3 = 0x3;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc1 = 0x1;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc2 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc3 = 0x3;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc4 = 0x4;
	regReqMsg->_5gmobileId.suci5gMobileId.spare1 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.spare2 = 0x0;

	regReqMsg->_5gregType.len = 2;
	regReqMsg->_5gregType.value = initialRegistration;
	/* ueSecuCapability TLV */
	//NOTE:Below direct structure copy does not work
	//regReqMsg->ueSecuCapabilityIE.secuCapability = testUe.secContext.ueSecurityCapability;
	regReqMsg->ueSecuCapability.len = 6 ;
	regReqMsg->ueSecuCapability._5gNASIntAlgo 	= testUe.secContext.ueSecurityCapability._5gNASIntAlgo;
	regReqMsg->ueSecuCapability._5gNASEncAlgo 	= testUe.secContext.ueSecurityCapability._5gNASEncAlgo;
	regReqMsg->ueSecuCapability._5gASIntAlgo 	= testUe.secContext.ueSecurityCapability._5gASIntAlgo;
	regReqMsg->ueSecuCapability._5gASEncAlgo 	= testUe.secContext.ueSecurityCapability._5gASEncAlgo;

	regReqMsg->presenceMask = 0;
	regReqMsg->presenceMask |= REGISTRATION_REQUEST_UE_SEC_CAPABILITY_PRESENT;

	uint32_t encodedLen = 0;
	if(nasMessageEncode((uint8_t *)test_nas_pdu,MAX_MESSAGE_SIZE,&nasMsg,
												&encodedLen,NULL) ==FAILURE) {
		higLog("%s"," nasMessageEncode() failed");
		return;
	}
	
	test_nas_pdu_len = encodedLen;
	OCTET_STRING_fromBuf(&nasPdu_IE->value.choice.NAS_PDU, test_nas_pdu,
							test_nas_pdu_len);
	rc = ASN_SEQUENCE_ADD(&initUEmsg->protocolIEs.list, nasPdu_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* 3. User Location Info */
		/*FIXME later: crashes on constraint check.
		 * UserLocationInformation is anyway ignored for now. */
	//userLocationInformation_IE = (ProtocolIE_Field_t *)
	//			calloc(1, sizeof(ProtocolIE_Field_t));
	//if(userLocationInformation_IE == NULL) {
	//	higLog("%s","calloc failed");
	//	return;
	//}
	//userLocationInformation_IE->id = 130;
	//			/*asn_VAL_15_id_UserLocationInformation*/
	//userLocationInformation_IE->value.present =
	//		ProtocolIE_Field__value_PR_UserLocationInformation;
	///*	3.1 choose NR & fill in its internals*/
	//usrLocInfo = &userLocationInformation_IE->value.choice.UserLocationInformation;
	//usrLocInfo->present = UserLocationInformation_PR_userLocationInformationNR;
	//usrLocInfoNR = &usrLocInfo->choice.userLocationInformationNR;
	///*	3.2.1 nR_CGI.pLMNIdentity  */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->nR_CGI.pLMNIdentity,
	//				test_plmnid, strlen(test_plmnid));
	///*	3.2.2 nR_CGI.nRCellIdentity  */
	//	/*TODO maybe write a small BITSTRING_fromBuf helper. cleaner */
	//usrLocInfoNR->nR_CGI.nRCellIdentity.buf = (uint8_t*)
	//				calloc(test_bits_len, sizeof(uint8_t));
	//memcpy(usrLocInfoNR->nR_CGI.nRCellIdentity.buf, test_bits, test_bits_len);
	//usrLocInfoNR->nR_CGI.nRCellIdentity.size = test_bits_len;
	//usrLocInfoNR->nR_CGI.nRCellIdentity.bits_unused =
	//					test_bits_len*8 - test_bits_num;
	///*	3.2.3 nR_CGI.iE_Extensions optional, so meh */
	///*	3.3.1 tAI.pLMNIdentity */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->tAI.pLMNIdentity,
	//				test_plmnid, strlen(test_plmnid));
	///*	3.3.2 tAI.TAC_t */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->tAI.tAC,
	//				test_tac, strlen(test_tac));
	///*	3.3.3 taI.iE_Extensions optional, so meh */
	///*	3.4 iE_Extensions optional, so meh */
	///*	3.5 wrap it up, add it to seq  */
	//rc = ASN_SEQUENCE_ADD(&initUEmsg->protocolIEs.list,
	//					userLocationInformation_IE);
	//if(rc) {
	//	higLog("ASN_SEQUENCE_ADD failed");
	//	return;
	//}

	/*check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0)
		higLog("check constraints failed: %s", errBuf);

	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	platformSCTPSendData(fdd->listenFD,msg, encRet.encoded, NULL,
                         UE_MESSAGE_STREAM);
	ngapFree(ngapPdu);
}



int testRecvSecurityModeCommand(struct fdData *fdd, testUeContext_t testUe)
{
	LOG_ENTRY;
	char msg[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_dec_rval_t decRet;
	int msgLen, rc=FAILURE;

	if((msgLen = platformRecvData(fdd->listenFD, msg)) <= 0)
		return rc;
	/*decode msg into NGAP PDU*/
	decRet = ber_decode(0, &asn_DEF_NGAP_PDU,
					(void**) &ngapPdu, msg, msgLen);
	if(decRet.code == RC_OK) {
		lowLog("%s", "Decode succeeded");
		asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	} else {
		higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
						"RC_FAIL" : "RC_WMORE");
		return rc;
	}

	higLog("Security mode command msg successfully received at UE!");

	/* copy values into ue context*/
	/*testUe->ranUeNgapId = ngapGetRanUeNgapId(ngapPdu);
	testUe->amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);*/

	//get and decode NAS
	NAS_PDU_t *nasPdu = ngapGetNasPdu(ngapPdu);

	nasMessage_t nasMsg = {};
	//Passing UE security context to check message integrity i.e match NAS MAC
	rc = nasMessageDecode((uint8_t*) nasPdu->buf, &nasMsg, nasPdu->size, &testUe.secContext);
	if (rc==FAILURE) {
		higLog("NAS Message Decode Failed");
		LOG_EXIT;
		return rc;
	} else if ( nasMsg.header.epd != _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
		higLog("Incorrect NAS Message type ");
		LOG_EXIT;
		return rc;
	} else if ( nasMsg.plain._5gmmMsg.mmheader.msgType != SECURITY_MODE_COMMAND) {
		higLog("Incorrect NAS Message type ");
		LOG_EXIT;
		return rc;
	}

	SecurityModeCommand_t *secModeCmdMsg = &(nasMsg.plain._5gmmMsg.secModeCmdMsg);
	if(secModeCmdMsg == NULL) {
		higLog("%s"," Invalid secModeCmdMsg pointer!");
		return rc;
	}

	/*Match below parameters from secModeCmdMsgbelow with testUe ones:
	1.ngKSI: compared below
	2.Replayed UE security capabilities: already done while decode.
	 decode can be successful only if protected message from AMF is decoded 
	 with matching testUe.secContext.ueSecurityCapability parameteters
	*/
	if(secModeCmdMsg->ngKsi.tsc != testUe.secContext.ngKSI.tsc ||
		secModeCmdMsg->ngKsi.ngksi != testUe.secContext.ngKSI.ngksi)
	{
		higLog("%s","ngKSI mismatched between UE and AMF!");
		LOG_EXIT;
		return rc;
	}

	/*TODO: ngapFree here WAS causing crash with below error:
								munmap_chunk(): invalid pointer
								Aborted (core dumped)
		Fixed issue in NASIntegrityNIA,and above crash was gone!*/

	ngapFree(ngapPdu);
	LOG_EXIT;
	return SUCCESS;
}


//TODO:Optimize with common ngapSendMessage method to send to AMF
/* Sending Security mode complete message to AMF which is UNPROTECTED as of now*/
void testSendSecurityModeComplete(struct fdData *fdd, testUeContext_t testUe)
{
	LOG_ENTRY;
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_enc_rval_t encRet;
	int rc;
	size_t errlen = MAX_MESSAGE_SIZE;
	InitiatingMessage_t  *initiatingMsg = 0;
	UplinkNASTransport_t  *uplinkNasTransport = 0;
	/* the IEs we will be  filling */
	ProtocolIE_Field_t *ranUeNgapId_IE = 0;
	ProtocolIE_Field_t *amfUeNgapId_IE = 0;
	ProtocolIE_Field_t *nasPdu_IE = 0;
	uint8_t test_nas_pdu[MAX_MESSAGE_SIZE] = {};
	uint32_t test_nas_pdu_len = 0;

	//temporary
	lowLog("testUe.ranUeNgapId: %d",testUe.ranUeNgapId);
	lowLog("testUe.amfUeNgapId: %d",testUe.amfUeNgapId);
	
	//TODO free calloc'ed data on failure SEQ FREE
	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	/* choose initiatingMessage */
	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	/* it is an Uplink NAS msg */
	initiatingMsg->procedureCode = 43; /* asn_VAL_44_id_UplinkNASTransport */
	initiatingMsg->value.present =
				InitiatingMessage__value_PR_UplinkNASTransport;
	uplinkNasTransport = &initiatingMsg->value.choice.UplinkNASTransport;

	/* start filling following fields */

	/* 1. AMF-UE-NGAP-ID */
	amfUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(amfUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	amfUeNgapId_IE->id = 12;              /* asn_VAL_1_id_AMF_UE_NGAP_ID */
	amfUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_AMF_UE_NGAP_ID;
	amfUeNgapId_IE->value.choice.AMF_UE_NGAP_ID = testUe.amfUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, amfUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}

	/* 2. RAN-UE-NGAP-ID */
	ranUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(ranUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	ranUeNgapId_IE->id = 91;              /* asn_VAL_1_id_RAN_UE_NGAP_ID */
	ranUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_RAN_UE_NGAP_ID;
	ranUeNgapId_IE->value.choice.RAN_UE_NGAP_ID = testUe.ranUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, ranUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}


	/* 3. NAS-PDU */
	nasPdu_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(nasPdu_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	nasPdu_IE->id = 40;                          /* asn_VAL_4_id_NAS_PDU */
	nasPdu_IE->value.present = ProtocolIE_Field__value_PR_NAS_PDU;
		/* 2.1 fill up NAS PDU*/
		/* fill up an AuthenticationResponseMsg_t */
	nasMessage_t nasMsg = {};
	SecurityModeComplete_t &secModeCompleteMsg = nasMsg.plain._5gmmMsg.secModeCompMsg;
	secModeCompleteMsg.mmHeader.epd = _5GS_MOBILITY_MANAGEMENT_MESSAGE;
	//TODO:Manage for SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED
	secModeCompleteMsg.mmHeader.secuHeader = SECURITY_HEADER_TYPE_NOT_PROTECTED;
	secModeCompleteMsg.mmHeader.msgType = SECURITY_MODE_COMPLETE;

	nasMessageEncode(test_nas_pdu, MAX_MESSAGE_SIZE, &nasMsg,
				&test_nas_pdu_len, NULL);
		/* encode AuthenticationResponseMsg into test_nas_pdu */
	OCTET_STRING_fromBuf(&nasPdu_IE->value.choice.NAS_PDU,
			(char *) test_nas_pdu, test_nas_pdu_len);
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, nasPdu_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}

	/* check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0) {
		higLog("check constraints failed: %s", errBuf);
		ngapFree(ngapPdu);
		return;
	}

	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
		ngapFree(ngapPdu);
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	/* Send an uplink NAS message*/
	platformSendData(fdd->listenFD, msg, encRet.encoded, NULL);
	ngapFree(ngapPdu);
}

//put all UE related values inside single function to initialize UE context
void initUeContext(testUeContext_t &testUe)
{
	LOG_ENTRY;
	supi_t supi={};
	supi.mcc = 0x7 & 0x0F;
	supi.mnc = 0x7 & 0x0F;
	supi.msin_high = 0x1 & 0x0F;
	supi.msin_low = 0xBB;//8 bit as expected
	testUe.supi = supi;
	/* ueSecuCapability LV */
	testUe.secContext.ueSecurityCapability.len = 4;
	testUe.secContext.ueSecurityCapability._5gNASEncAlgo = NAS_ENCRYPTION_ALG_NEA2;
	testUe.secContext.ueSecurityCapability._5gNASIntAlgo = NAS_INTEGRITY_ALG_NIA2;
	testUe.secContext.ueSecurityCapability._5gASEncAlgo = 0x00;
	testUe.secContext.ueSecurityCapability._5gASIntAlgo = 0x00;
	testUe.secContext.direction_decode = SECU_DIRECTION_DOWNLINK;
	testUe.secContext.direction_encode = SECU_DIRECTION_UPLINK;
	memset(&testUe.secContext.dl_count, 0, 4);
	memset(&testUe.secContext.ul_count, 0, 4);
	LOG_EXIT;
}


#if 0
//NOTE: This is just a temporary function to calculate all UE side security keys and RES*
int handleUeSecurityParams(testUeContext_t &testUe)
{
	LOG_ENTRY;
	int rc = FAILURE;
	uint8_t ABBA = 0x00;//Refer to AMF, value kept same
	/*TODO: Below code is for actual computations of UE response & NF keys at AUSF & UDM
	and needs to be enabled once ausf & udm code integrated with NRF*/

	uint8_t SQN[SQN_SIZE]={}; //48 bit
	uint8_t AK[AK_SIZE]={}; //48 bit
	//uint8_t MAC_A[MAC_SIZE]={};//64 bits
	uint8_t IK[IK_SIZE]={};//128 bits
	uint8_t CK[CK_SIZE]={};//128 bits
	uint8_t RES[RES_SIZE]={}; //64 bits
	uint8_t Kausf[KAUSF_SIZE]={}; //256 bits
	uint8_t Kseaf[KSEAF_SIZE]={}; //256 bits

	//uint8_t RESStar[XRES_SIZE]={}; //128 bits

	char snName[4] = {};
	strcpy(snName, "amf");


	//Below values taken from UDM as it is for UE, since this is temporary
	//Ultimately RAN emulator will need to store fetch values from USIM and use for computations
	uint8_t KA[KA_SIZE] ={0x34, 0xb7, 0xd5, 0xee, 0xe1, 0x1a, 0x21, 0x62,
						0xbb, 0x20, 0xf3, 0xc4, 0xe5, 0x77, 0x36, 0x74};
	uint8_t op[OP_SIZE] ={0x63, 0xbf, 0xa5, 0x0e, 0xe6, 0x52, 0x33, 0x65,
						  0xff, 0x14, 0xc1, 0xf4, 0x5f, 0x88, 0x73, 0x7d};

	//Enable below code to test with actual computed values from UDM

	//Step1: Calculate keys using f2,f3,f4,f5, ensure you get CORRECT value of RAND from AMF
	f2345(KA, op, testUe.secContext._5gAv.RAND, RES, AK, CK, IK);

	//Step2:Extract first 6 byes equivalent => SQN ^ AK
	uint8_t tempEXOR[SQN_SIZE]={};
	memcpy(tempEXOR , testUe.secContext._5gAv.AUTN, SQN_SIZE);

	int index=0;
	for(index=0; index<SQN_SIZE; index++)
		SQN[index]= tempEXOR[index] ^ AK[index];
	lowLog("%s","SQN:");
	printHex(SQN,SQN_SIZE);

	//f1(KA, op, testUe.RAND, RES, SQN , MAC_A);

	//Step3: Compute RES* from RES
	rc = computeXRES(testUe.secContext._5gAv.RAND, CK, IK, RES, snName, testUe.secContext._5gAv.RESstar);
	if(rc == FAILURE)
	{
		higLog("%s","computeXRES function failed!!");
		LOG_EXIT;
		return rc;
	}
	lowLog("%s","RES*:");
	printHex(testUe.secContext._5gAv.RESstar, XRES_SIZE);

	//Step4: Calculate Kausf using CK || IK as a key, SQN^AK||snName as input
	rc = computeKausf(CK,IK,SQN,AK,snName,Kausf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKausf failed!!");
		LOG_EXIT;
		return rc;
	}


	//Step5: Calculate Kseaf
	rc = computeKseaf(Kausf, snName, Kseaf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKseaf failed!");
		LOG_EXIT;
		return rc;
	}
	//print_SUPI(testUe.supi);
	//Temporarily matching same Kseaf as in AMF BUT doing all computations that are part of AMF
	uint8_t Kseaf[KSEAF_SIZE]={0x7e, 0x03, 0x3c, 0x21, 0xe3, 0x49, 0x3c, 0x7e,
							   0x03, 0x5d, 0x49, 0x21, 0x03, 0x7d, 0xaa, 0xff,
							   0x5e, 0x33, 0xc5, 0x16, 0xdd, 0x44, 0xf3, 0xe9,
							   0x08, 0x4c, 0xcc, 0x61, 0x01, 0x77, 0x1a, 0xef};

	//Step6: Calculate Kamf
	//initializing Kamf precisely
	memset(testUe.Kamf,0,KAMF_SIZE);

	rc = computeKamf( Kseaf, testUe.supi, ABBA, testUe.Kamf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKamf failed");
		LOG_EXIT;
		return rc;
	}

	//Step7: Calculate KNASint
	uint8_t algo_id = testUe.secContext.ueSecurityCapability._5gNASIntAlgo; //NAS_INTEGRITY_ALG_NIA2;
	uint8_t algo_distinguisher = N_NAS_INTEGRITY_ALG; //algo type distinguisher for N-NAS-int-algo
	rc = computeNasInt(testUe.Kamf, algo_distinguisher, algo_id, 
						testUe.secContext.K_NASint);
	if(rc == FAILURE)
	{
		higLog("%s","computation of NAS integrity key failed");
		LOG_EXIT;
		return rc;
	}
	lowLog("%s","K_NASint:");
	printHex(testUe.secContext.K_NASint, NAS_KEY_SIZE);
	//Step7: Calculate KNASenc
	algo_id = testUe.secContext.ueSecurityCapability._5gNASEncAlgo;//NAS_ENCRYPTION_ALG_NEA2
	algo_distinguisher = N_NAS_ENCRYPTION_ALG;//algo type distinguisher for N-NAS-enc-algo
	rc = computeNasEnc(testUe.Kamf, algo_distinguisher, algo_id,
		testUe.secContext.K_NASenc);
	if(rc == FAILURE)
	{
		higLog("Generation of K_NASint key failed!");
		LOG_EXIT;
		return rc;
	}

	LOG_EXIT;
	return SUCCESS;
}
#endif

/*  TODO send valid values, free calloc'ed structs  */
//NOT USED AS OF NOW
#if 0
void testSendUplinkNASTransport(struct fdData * fdd)
{
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	int rc;
	size_t errlen = MAX_MESSAGE_SIZE;
	asn_enc_rval_t encRet;
	NGAP_PDU_t  *ngapPdu = 0;
	InitiatingMessage_t  *initiatingMsg = 0;
	UplinkNASTransport_t  *uplinkNasTransport = 0;
	/* the IEs we will be  filling */
	ProtocolIE_Field_t *ranUeNgapId_IE = 0;
	ProtocolIE_Field_t *amfUeNgapId_IE = 0;
	ProtocolIE_Field_t *nasPdu_IE = 0;
	char test_nas_pdu[MAX_MESSAGE_SIZE];
	uint32_t test_nas_pdu_len = 0;

	/*an elaborate field, commented out for now */
	//ProtocolIE_Field_t *userLocationInformation_IE = 0;
	//UserLocationInformation_t *usrLocInfo = 0;
	//UserLocationInformationNR_t *usrLocInfoNR = 0;
	//char test_plmnid[] = "1234";
	//char test_tac[] = "567";
	//uint8_t test_bits[] = {0xF1, 0x20}; /* = FF 2, 12 bits last 4 bits empty*/
	//int test_bits_len = 2;
	//int test_bits_num = 12;

	//TODO free calloc'ed data on failure SEQ FREE
	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	/* choose initiatingMessage */
	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	/* it is a initial UE msg */
	initiatingMsg->procedureCode = 43; /* asn_VAL_29_id_InitialUEMessage */
	initiatingMsg->value.present =
				InitiatingMessage__value_PR_UplinkNASTransport;
	uplinkNasTransport = &initiatingMsg->value.choice.UplinkNASTransport;

	/* start filling following fields */

	/* 1. AMF-UE-NGAP-ID */
	amfUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(amfUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	amfUeNgapId_IE->id = 12;              /* asn_VAL_1_id_AMF_UE_NGAP_ID */
	amfUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_AMF_UE_NGAP_ID;
	amfUeNgapId_IE->value.choice.AMF_UE_NGAP_ID = 1;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, amfUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* 2. RAN-UE-NGAP-ID */
	ranUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(ranUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	ranUeNgapId_IE->id = 91;              /* asn_VAL_1_id_RAN_UE_NGAP_ID */
	ranUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_RAN_UE_NGAP_ID;
	ranUeNgapId_IE->value.choice.RAN_UE_NGAP_ID = 100;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list,ranUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* 3. NAS-PDU */
	nasPdu_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(nasPdu_IE == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	nasPdu_IE->id = 40;                          /* asn_VAL_4_id_NAS_PDU */
	nasPdu_IE->value.present = ProtocolIE_Field__value_PR_NAS_PDU;
		/* 2.1 fill up NAS PDU*/

	nasMessage_t	nasMsg;	
	RegistrationRequest_t *regReqMsg = &(nasMsg.plain._5gmmMsg.regReqMsg);
	regReqMsg->mmHeader.epd 			= _5GS_MOBILITY_MANAGEMENT_MESSAGE;
	regReqMsg->mmHeader.secuHeader 	= SECURITY_HEADER_TYPE_NOT_PROTECTED;
	regReqMsg->mmHeader.msgType 		= REGISTRATION_REQUEST;
	regReqMsg->ngKsi.tsc 	= 0;
	regReqMsg->ngKsi.ngksi	= 0x07;
	
	regReqMsg->_5gmobileId.len = 10;
	regReqMsg->_5gmobileId.suci5gMobileId.supiFormat = E_IMSI;
	regReqMsg->_5gmobileId.suci5gMobileId.oddEven = _5G_MOBILE_IDENTITY_EVEN;
	regReqMsg->_5gmobileId.suci5gMobileId.identityType = E_SUCI;
	regReqMsg->_5gmobileId.suci5gMobileId.protectionSchId = E_NULL_SCHEME;
	regReqMsg->_5gmobileId.suci5gMobileId.homeNtwrkPKI = 0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput1 = 0x9;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput2 = 0x1;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput3 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput4 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput5 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput6 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput7 = 0x0;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput8 = 0x4;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput9 = 0x4;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput10 = 0x5;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput11 = 0x8;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput12 = 0x6;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput13 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput14 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.schemeOutput15 = 0x2;
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit1 = 0x01; 
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit2 = 0x02;
	regReqMsg->_5gmobileId.suci5gMobileId.mccDigit3 = 0x03;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit1 = 0x01;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit2 = 0x02;
	regReqMsg->_5gmobileId.suci5gMobileId.mncDigit3 = 0x03;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc1 = 0x01;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc2 = 0x02;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc3 = 0x03;
	regReqMsg->_5gmobileId.suci5gMobileId.routingInc4 = 0x04;

	regReqMsg->_5gregType.len = 2;
	regReqMsg->_5gregType.value = initialRegistration;
	regReqMsg->ueSecuCapability._5gEncrypAlgo = NAS_ENCRYPTION_ALG_NEA2;
	regReqMsg->ueSecuCapability._5gIntgAlgo = NAS_INTEGRITY_ALG_NIA2;
	regReqMsg->ueSecuCapability.EpsEncrypAlgo = 0x00;
	regReqMsg->ueSecuCapability.EpsIntgAlgo = 0x00;
	uint32_t encodedLen = 0;
	if(nasMessageEncode((uint8_t *)test_nas_pdu,MAX_MESSAGE_SIZE,&nasMsg,
												&encodedLen,NULL) ==FAILURE) {
		higLog("%s"," nasMessageEncode() failed");
		return;
	}	

	test_nas_pdu_len = encodedLen;

	OCTET_STRING_fromBuf(&nasPdu_IE->value.choice.NAS_PDU, test_nas_pdu,
							test_nas_pdu_len);
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, nasPdu_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* 4. User Location Info */
		/*FIXME later: crashes on constraint check.
		 * UserLocationInformation is anyway ignored for now. */
	//userLocationInformation_IE = (ProtocolIE_Field_t *)
	//			calloc(1, sizeof(ProtocolIE_Field_t));
	//if(userLocationInformation_IE == NULL) {
	//	higLog("%s","calloc failed");
	//	return;
	//}
	//userLocationInformation_IE->id = 130;
	//			/*asn_VAL_15_id_UserLocationInformation*/
	//userLocationInformation_IE->value.present =
	//		ProtocolIE_Field__value_PR_UserLocationInformation;
	///*	3.1 choose NR & fill in its internals*/
	//usrLocInfo = &userLocationInformation_IE->value.choice.UserLocationInformation;
	//usrLocInfo->present = UserLocationInformation_PR_userLocationInformationNR;
	//usrLocInfoNR = &usrLocInfo->choice.userLocationInformationNR;
	///*	3.2.1 nR_CGI.pLMNIdentity  */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->nR_CGI.pLMNIdentity,
	//				test_plmnid, strlen(test_plmnid));
	///*	3.2.2 nR_CGI.nRCellIdentity  */
	//	/*TODO maybe write a small BITSTRING_fromBuf helper. cleaner */
	//usrLocInfoNR->nR_CGI.nRCellIdentity.buf = (uint8_t*)
	//				calloc(test_bits_len, sizeof(uint8_t));
	//memcpy(usrLocInfoNR->nR_CGI.nRCellIdentity.buf, test_bits, test_bits_len);
	//usrLocInfoNR->nR_CGI.nRCellIdentity.size = test_bits_len;
	//usrLocInfoNR->nR_CGI.nRCellIdentity.bits_unused =
	//					test_bits_len*8 - test_bits_num;
	///*	3.2.3 nR_CGI.iE_Extensions optional, so meh */
	///*	3.3.1 tAI.pLMNIdentity */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->tAI.pLMNIdentity,
	//				test_plmnid, strlen(test_plmnid));
	///*	3.3.2 tAI.TAC_t */
	//OCTET_STRING_fromBuf((OCTET_STRING_t*) &usrLocInfoNR->tAI.tAC,
	//				test_tac, strlen(test_tac));
	///*	3.3.3 taI.iE_Extensions optional, so meh */
	///*	3.4 iE_Extensions optional, so meh */
	///*	3.5 wrap it up, add it to seq  */
	//rc = ASN_SEQUENCE_ADD(&initUEmsg->protocolIEs.list,
	//					userLocationInformation_IE);
	//if(rc) {
	//	higLog("ASN_SEQUENCE_ADD failed");
	//	return;
	//}

	/*check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0)
		higLog("check constraints failed: %s", errBuf);

	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	platformSCTPSendData(fdd->listenFD,msg, encRet.encoded, NULL,
                         UE_MESSAGE_STREAM);
	ngapFree(ngapPdu);
}
#endif

/* Send an authentication response. hardcoded RES is sent. */
void testSendAuthResponse(struct fdData *fdd, testUeContext_t testUe)
{
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_enc_rval_t encRet;
	int rc;
	size_t errlen = MAX_MESSAGE_SIZE;
	InitiatingMessage_t  *initiatingMsg = 0;
	UplinkNASTransport_t  *uplinkNasTransport = 0;
	/* the IEs we will be  filling */
	ProtocolIE_Field_t *ranUeNgapId_IE = 0;
	ProtocolIE_Field_t *amfUeNgapId_IE = 0;
	ProtocolIE_Field_t *nasPdu_IE = 0;
	uint8_t test_nas_pdu[MAX_MESSAGE_SIZE] = {};
	uint32_t test_nas_pdu_len = 0;

	lowLog("testUe.ranUeNgapId: %d",testUe.ranUeNgapId);
	lowLog("testUe.amfUeNgapId: %d",testUe.amfUeNgapId);

	//TODO free calloc'ed data on failure SEQ FREE
	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return;
	}
	/* choose initiatingMessage */
	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	/* it is an Uplink NAS msg */
	initiatingMsg->procedureCode = 43; /* asn_VAL_44_id_UplinkNASTransport */
	initiatingMsg->value.present =
				InitiatingMessage__value_PR_UplinkNASTransport;
	uplinkNasTransport = &initiatingMsg->value.choice.UplinkNASTransport;

	/* start filling following fields */

	/* 1. AMF-UE-NGAP-ID */
	amfUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(amfUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	amfUeNgapId_IE->id = 12;              /* asn_VAL_1_id_AMF_UE_NGAP_ID */
	amfUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_AMF_UE_NGAP_ID;
	amfUeNgapId_IE->value.choice.AMF_UE_NGAP_ID = testUe.amfUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, amfUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}

	/* 2. RAN-UE-NGAP-ID */
	ranUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(ranUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	ranUeNgapId_IE->id = 91;              /* asn_VAL_1_id_RAN_UE_NGAP_ID */
	ranUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_RAN_UE_NGAP_ID;
	ranUeNgapId_IE->value.choice.RAN_UE_NGAP_ID = testUe.ranUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, ranUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}


	/* 3. NAS-PDU */
	nasPdu_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(nasPdu_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return;
	}
	nasPdu_IE->id = 40;                          /* asn_VAL_4_id_NAS_PDU */
	nasPdu_IE->value.present = ProtocolIE_Field__value_PR_NAS_PDU;
		/* 2.1 fill up NAS PDU*/
		/* fill up an AuthenticationResponseMsg_t */
	nasMessage_t nasMsg = {};
	AuthenticationResponseMsg_t &authRespMsg = nasMsg.plain._5gmmMsg.authRespMsg;
	authRespMsg.mmHeader.epd = _5GS_MOBILITY_MANAGEMENT_MESSAGE;
	authRespMsg.mmHeader.secuHeader = SECURITY_HEADER_TYPE_NOT_PROTECTED;
	authRespMsg.mmHeader.msgType = AUTHENTICATION_RESPONSE;
	authRespMsg.authRespParam.arpLen = XRES_SIZE + AUTH_RESP_PARAM_HEADER_LEN;
                                    /* XRES_SIZE  + 2 octets arpIEI & arpLen*/
	memcpy(authRespMsg.authRespParam.RESstar, testUe.secContext._5gAv.RESstar,
            XRES_SIZE);
    authRespMsg.presenceMask = 0;
    authRespMsg.presenceMask |= NAS_AUTH_RESP_OPT_AUTH_RESP_PARAM_PRESENT;
	nasMessageEncode(test_nas_pdu, MAX_MESSAGE_SIZE, &nasMsg,
				&test_nas_pdu_len, NULL);
		/* encode AuthenticationResponseMsg into test_nas_pdu */
	OCTET_STRING_fromBuf(&nasPdu_IE->value.choice.NAS_PDU,
			(char *) test_nas_pdu, test_nas_pdu_len);
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, nasPdu_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return;
	}

	/* check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0) {
		higLog("check constraints failed: %s", errBuf);
		ngapFree(ngapPdu);
		return;
	}

	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
		ngapFree(ngapPdu);
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	/* Send an uplink NAS message*/
	platformSCTPSendData(fdd->listenFD, msg, encRet.encoded, NULL,
                         UE_MESSAGE_STREAM);
	ngapFree(ngapPdu);
}



void testRecvNGSetupResponse(struct fdData *fdd) {
	// eat this message 
	LOG_ENTRY;
	char msg[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_dec_rval_t decRet;
    int msgLen;

	if((msgLen = platformRecvData(fdd->listenFD, msg)) <= 0)
		return;

	decRet = ber_decode(0, &asn_DEF_NGAP_PDU,
					(void**) &ngapPdu, msg, msgLen);
	if(decRet.code == RC_OK) {
		lowLog("%s", "Decode succeeded");
		asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	} else {
		higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
						"RC_FAIL" : "RC_WMORE");
		return;
	}

    ngapFree(ngapPdu);
	LOG_EXIT;
}


void sigpipe_handler(int unused)
{
	midLog("%s", "received SIGPIPE signal. maybe the server died");
}


int testSendRegCompleteMsg(struct fdData *fdd, testUeContext_t testUe) {
	
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_enc_rval_t encRet;
	int rc = FAILURE;
	size_t errlen = MAX_MESSAGE_SIZE;
	InitiatingMessage_t  *initiatingMsg = 0;
	UplinkNASTransport_t  *uplinkNasTransport = 0;
	/* the IEs we will be  filling */
	ProtocolIE_Field_t *ranUeNgapId_IE = 0;
	ProtocolIE_Field_t *amfUeNgapId_IE = 0;
	ProtocolIE_Field_t *nasPdu_IE = 0;
	/* nas message will be encoded here */
	uint8_t test_nas_pdu[MAX_MESSAGE_SIZE] = {};
	uint32_t test_nas_pdu_len = 0;

	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return FAILURE;
	}
	/* choose initiatingMessage */
	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	/* it is an Uplink NAS msg */
	initiatingMsg->procedureCode = 43; /* asn_VAL_44_id_UplinkNASTransport */
	initiatingMsg->value.present =
				InitiatingMessage__value_PR_UplinkNASTransport;
	uplinkNasTransport = &initiatingMsg->value.choice.UplinkNASTransport;

	/* start filling following fields */

	/* 1. AMF-UE-NGAP-ID */
	amfUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(amfUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}
	amfUeNgapId_IE->id = 12;              /* asn_VAL_1_id_AMF_UE_NGAP_ID */
	amfUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_AMF_UE_NGAP_ID;
	amfUeNgapId_IE->value.choice.AMF_UE_NGAP_ID = testUe.amfUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, amfUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}

	/* 2. RAN-UE-NGAP-ID */
	ranUeNgapId_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(ranUeNgapId_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}
	ranUeNgapId_IE->id = 91;              /* asn_VAL_1_id_RAN_UE_NGAP_ID */
	ranUeNgapId_IE->value.present = ProtocolIE_Field__value_PR_RAN_UE_NGAP_ID;
	ranUeNgapId_IE->value.choice.RAN_UE_NGAP_ID = testUe.ranUeNgapId;
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, ranUeNgapId_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}


	/* 3. NAS-PDU */
	nasPdu_IE = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));
	if(nasPdu_IE == NULL) {
		higLog("%s","calloc failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}
	nasPdu_IE->id = 40;                          /* asn_VAL_4_id_NAS_PDU */
	nasPdu_IE->value.present = ProtocolIE_Field__value_PR_NAS_PDU;
	
	/* 2.1 fill up NAS PDU*/
	/* fill up an  */

	
	nasMessage_t nasMsg = {};
	
//	nasMessageSecurityProtected *nasSecProtected = &(nasMsg.security_protected);
//	nasSecProtected->header.epd 		= _5GS_MOBILITY_MANAGEMENT_MESSAGE;
//	nasSecProtected->header.secuHeader 	=  SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED;
//	nasSecProtected->header.sqnNum		= 0x07;	/* TO DO */

	RegistrationCompleteMsg_t *regComplMsg = &(nasMsg.plain._5gmmMsg.regComplMsg);

	regComplMsg->mmHeader.epd 		= _5GS_MOBILITY_MANAGEMENT_MESSAGE;
	regComplMsg->mmHeader.secuHeader =
                            SECURITY_HEADER_TYPE_NOT_PROTECTED;
                //FIXME  :  SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CIPHERED;
	regComplMsg->mmHeader.msgType 	= REGISTRATION_COMPLETE;


	/*
		here we need some nas security that UE stores 
		It uses this context while encoding and decoding
		and other security related work

		for now testUe's security context is used
	*/

	rc = nasMessageEncode(test_nas_pdu,MAX_MESSAGE_SIZE,&nasMsg,&test_nas_pdu_len,&testUe.secContext);
	if(rc == FAILURE) {
		higLog("%s"," nasMessageEncode() failed");
		return rc;
	}
	/* encode AuthenticationResponseMsg into test_nas_pdu */
	OCTET_STRING_fromBuf(&nasPdu_IE->value.choice.NAS_PDU,
			(char *) test_nas_pdu, test_nas_pdu_len);
	rc = ASN_SEQUENCE_ADD(&uplinkNasTransport->protocolIEs.list, nasPdu_IE);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		ngapFree(ngapPdu);
		return FAILURE;
	}

	/* check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0) {
		higLog("check constraints failed: %s", errBuf);
		ngapFree(ngapPdu);
		return FAILURE;
	}

	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
		ngapFree(ngapPdu);
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	/* Send an uplink NAS message*/
	platformSCTPSendData(fdd->listenFD, msg, encRet.encoded, NULL,
                         UE_MESSAGE_STREAM);
	ngapFree(ngapPdu);
	return SUCCESS;
}

void testNgSetupMessage(struct fdData *fdd) {
	char msg[MAX_MESSAGE_SIZE], errBuf[MAX_MESSAGE_SIZE];
	NGAP_PDU_t  *ngapPdu = 0;
	asn_enc_rval_t encRet;
	int rc;
	size_t errlen = MAX_MESSAGE_SIZE;
	InitiatingMessage_t  *initiatingMsg = 0;
	NGSetupRequest_t	 *ngSetUpReq = 0;

	ProtocolIE_Field_t *globalRanId = 0;
	ProtocolIE_Field_t *taList = 0;
	ProtocolIE_Field_t *ranNodeName = 0;

	ngapPdu = (NGAP_PDU_t*) calloc(1, sizeof(NGAP_PDU_t));
	if(ngapPdu == NULL) {
		higLog("%s","calloc failed");
		return;
	}

	ngapPdu->present = NGAP_PDU_PR_initiatingMessage;
	initiatingMsg = &ngapPdu->choice.initiatingMessage;

	initiatingMsg->procedureCode = 21; /* ProcedureCode ::= 21 : 38.413 */
	initiatingMsg->value.present = 
				InitiatingMessage__value_PR_NGSetupRequest;
	ngSetUpReq = &initiatingMsg->value.choice.NGSetupRequest;	
		/* here is the pointer to the actual ngap message */

	globalRanId = (ProtocolIE_Field_t *)
				calloc(1, sizeof(ProtocolIE_Field_t));	
	globalRanId->id = 28; /* asn_VAL_100_id_GlobalRANNodeID */
	//globalRanId->id = 1; /* asn_VAL_100_id_GlobalRANNodeID */
    /* CRASHING IF ID IS 1. ???? FIXME */
	globalRanId->value.present = ProtocolIE_Field__value_PR_GlobalRANNodeID;
	

	GlobalRANNodeID_t *globalRid = &globalRanId->value.choice.GlobalRANNodeID; 
		/* GlobalRANNodeID is again a struct */
	globalRid->present = GlobalRANNodeID_PR_globalGNB_ID;
	uint32_t thisRanGnbId = 100;	/* How to get this value ? TODO */
	
	globalRid->choice.globalGNB_ID.gNB_ID.present = GNB_ID_PR_gNB_ID;
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf = (uint8_t*)calloc(3, sizeof(uint8_t));
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[0] = (thisRanGnbId) >> 12;
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[1] = thisRanGnbId >> 4;
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.buf[2] = (thisRanGnbId & 0x0f) << 4;
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.size = 3;
	globalRid->choice.globalGNB_ID.gNB_ID.choice.gNB_ID.bits_unused = 4;
	// gNB_ID is set //


	/* the following value should be retreived form a GNB profile or context stored in this GNB */
	// set the plmn ID
	// these variblaes needs to be defined somewhere specific to this RAN instance
	uint32_t tac = 0x112233;	/* TODO FIXME */
	/* Mobile Country Code
	* Mobile Network Code
	*/
	//uint16_t  mcc = 0x8888; /* Use this value to test NGSetupFailure */
	uint16_t  mcc = 0x7777; /* Use this value to test NGSetupResponse - successful case */
	uint16_t  mnc = 0x7778;  /* TODO  FIXME */
	uint8_t   mnc_digit_length = 0x50;	/* TODO  FIXME */

	char _buf[3];        
	_buf[0] = (MCC_MNC_DECIMAL(mcc) << 4) | MCC_HUNDREDS(mcc);       
	_buf[1] = (MNC_HUNDREDS(mnc,mnc_digit_length) << 4) | MCC_MNC_DIGIT(mcc);
	_buf[2] = (MCC_MNC_DIGIT(mnc) << 4) | MCC_MNC_DECIMAL(mnc);  
#if 0
	globalRid->choice.globalGNB_ID.pLMNIdentity.buf =
					(uint8_t*)calloc(3, sizeof(uint8_t));
	globalRid->choice.globalGNB_ID.pLMNIdentity.buf[0] =
					(MCC_MNC_DECIMAL(mcc) << 4) | MCC_HUNDREDS(mcc);
	globalRid->choice.globalGNB_ID.pLMNIdentity.buf[1] =
					(MNC_HUNDREDS(mnc,mnc_digit_length) << 4) |
					MCC_MNC_DIGIT(mcc);
	globalRid->choice.globalGNB_ID.pLMNIdentity.buf[2] =
					(MCC_MNC_DIGIT(mnc) << 4) | MCC_MNC_DECIMAL(mnc);
	globalRid->choice.globalGNB_ID.pLMNIdentity.size = 3;
#endif

	
	OCTET_STRING_fromBuf(&globalRid->choice.globalGNB_ID.pLMNIdentity, _buf, 3); 
	
	// plmnID set
	/* Now add this protocolIE_Field_t to the list */ 
	rc = ASN_SEQUENCE_ADD(&ngSetUpReq->protocolIEs.list, globalRanId);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}


	/* add supported TA list "SupportedTAList_t" */
	taList = (ProtocolIE_Field_t *)calloc(1,sizeof(ProtocolIE_Field_t));
	taList->id = 107; /* asn_VAL_109_id_SupportedTAList */
	taList->value.present = ProtocolIE_Field__value_PR_SupportedTAList;

	SupportedTAList_t *supportedTaList =  &taList->value.choice.SupportedTAList;
	/* 	
	typedef struct SupportedTAList {
		A_SEQUENCE_OF(struct SupportedTAItem) list;

		asn_struct_ctx_t _asn_ctx;
	} SupportedTAList_t; 
	
	*/
	/* define one TA_item */
	SupportedTAItem_t	 *taItem = (SupportedTAItem_t *)calloc(1, sizeof(SupportedTAItem_t));	/* add one item */
	/* 	
	typedef struct SupportedTAItem {
		TAC_t	 tAC;
		BroadcastPLMNList_t	 broadcastPLMNList;
		struct ProtocolExtensionContainer	*iE_Extensions;
		asn_struct_ctx_t _asn_ctx;
	} SupportedTAItem_t; 

	typedef struct BroadcastPLMNList {
		A_SEQUENCE_OF(struct BroadcastPLMNItem) list;
			
		asn_struct_ctx_t _asn_ctx;
	} BroadcastPLMNList_t; 
	
	*/

	/* first add the tac */
	taItem->tAC.size = 3;	/* constraint on size 3 */
	taItem->tAC.buf = (uint8_t *)calloc(3,sizeof(uint8_t));
	taItem->tAC.buf[0] = tac >> 16;
	taItem->tAC.buf[1] = tac >> 8;
	taItem->tAC.buf[2] = tac;
	// INT16_TO_OCTET_STRING(tac,&taItem.tAC);	/* tac size was 2 bytes in 4G*/
	
	/* add one BroadcastPLMNItem_t */
	BroadcastPLMNItem_t  *broadcastPLMNItem = (BroadcastPLMNItem_t *)calloc(1, sizeof(BroadcastPLMNItem_t));
	/* 
	
	typedef struct BroadcastPLMNItem {
		PLMNIdentity_t	 pLMNIdentity;
		SliceSupportList_t	 tAISliceSupportList;
		struct ProtocolExtensionContainer	*iE_Extensions;
		asn_struct_ctx_t _asn_ctx;
	} BroadcastPLMNItem_t; 
	
	*/

/*     char _buf[3];        
	_buf[0] = (MCC_MNC_DECIMAL(mcc) << 4) | MCC_HUNDREDS(mcc);       
	_buf[1] = (MNC_HUNDREDS(mnc,mnc_digit_length) << 4) | MCC_MNC_DIGIT(mcc);
	_buf[2] = (MCC_MNC_DIGIT(mnc) << 4) | MCC_MNC_DECIMAL(mnc);   */    
	OCTET_STRING_fromBuf(&broadcastPLMNItem->pLMNIdentity, _buf, 3); 	

	SliceSupportItem_t *slice1 = (SliceSupportItem_t *)calloc(1, sizeof(SliceSupportItem_t));	/* will not be used now */
	char sstBuf = 0;			/*correct value  TODO */
	char sdBuf[3];	
	memset(sdBuf,0,sizeof(sdBuf));	/*correct value  TODO */

	OCTET_STRING_fromBuf(&slice1->s_NSSAI.sST,&sstBuf,1);
	OCTET_STRING_fromBuf(&slice1->s_NSSAI.sD,sdBuf,3);
	
	rc = ASN_SEQUENCE_ADD(&broadcastPLMNItem->tAISliceSupportList.list, slice1);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}	

	rc = ASN_SEQUENCE_ADD(&taItem->broadcastPLMNList.list , broadcastPLMNItem);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* Now add this SupportedTAItem_t to the list of supportedTaList */ 
	rc = ASN_SEQUENCE_ADD(&supportedTaList->list , taItem);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}

	/* Now add this protocolIE_Field_t : "taList" to the main list */ 
	rc = ASN_SEQUENCE_ADD(&ngSetUpReq->protocolIEs.list, taList);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}



	/* add RAN NODE NAME  */
	ranNodeName = (ProtocolIE_Field_t *)calloc(1,sizeof(ProtocolIE_Field_t));
	ranNodeName->id = 88; 	/* asn_VAL_101_id_RANNodeName */
	ranNodeName->value.present = ProtocolIE_Field__value_PR_RANNodeName;
	
	//ProtocolIE_Field__value_PR_RANNodeName;
	//ProtocolIE_Field__value_PR_PDUSessionResourceModifyListModRes;
	
	
	//char *ranName = "RAN-0"; /* check RANNodeName.h for the valid characters */
	//OCTET_STRING_fromBuf(&ranNodeName->value.choice.RANNodeName,ranName,strlen(ranName));

	ranNodeName->value.choice.RANNodeName.size = 3;
	ranNodeName->value.choice.RANNodeName.buf = (uint8_t*)calloc(3,sizeof(uint8_t));
	ranNodeName->value.choice.RANNodeName.buf[0] = 'R';
	ranNodeName->value.choice.RANNodeName.buf[1] = 'A';
	ranNodeName->value.choice.RANNodeName.buf[2] = 'N';



	rc = ASN_SEQUENCE_ADD(&ngSetUpReq->protocolIEs.list, ranNodeName);
	if(rc) {
		higLog("ASN_SEQUENCE_ADD failed");
		return;
	}
	// everything added 


	/* check if everything is correct*/
	if(asn_check_constraints(&asn_DEF_NGAP_PDU, ngapPdu,
					errBuf, &errlen) < 0) {
		higLog("check constraints failed: %s", errBuf);
		//ngapFree(ngapPdu);
		//return;
	}
	// printing output in XML is failing (xer_fprint). TODO check.
	/* print what we have gathered, fill it into a buff & sendmsg()*/
	asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	encRet = der_encode_to_buffer(&asn_DEF_NGAP_PDU, ngapPdu,
				msg, MAX_MESSAGE_SIZE); //TODO init msg
	if(encRet.encoded == -1) {
		higLog("%s", "encode failed");
		ngapFree(ngapPdu);
	} else
		lowLog("Encoded %d bytes", encRet.encoded);

	/* Send an NGSetup message*/
	platformSCTPSendData(fdd->listenFD, msg, encRet.encoded, NULL,
                         NON_UE_MESSAGE_STREAM);
	// Showing error in ngapFree
	ngapFree(ngapPdu); /* FIXME */

}

void testRecvRegReject(struct fdData *fdd)
{
	char msg[MAX_MESSAGE_SIZE];
    int msgLen = 0, rc =0;
	NGAP_PDU_t  *ngapPdu = 0;
	asn_dec_rval_t decRet;

	if((msgLen = platformRecvData(fdd->listenFD, msg)) <= 0)
		return;

	/*decode msg into NGAP PDU*/
	decRet = ber_decode(0, &asn_DEF_NGAP_PDU,
					(void**) &ngapPdu, msg, msgLen);
	if(decRet.code == RC_OK) {
		lowLog("%s", "Decode succeeded");
		asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	} else {
		higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
						"RC_FAIL" : "RC_WMORE");
		return;
	}

	NAS_PDU_t *nasPdu = ngapGetNasPdu(ngapPdu);
    nasMessage_t nasMsg = {};
    rc = nasMessageDecode((uint8_t*) nasPdu->buf, &nasMsg, nasPdu->size, NULL);
    if (rc==FAILURE) {
        higLog("NAS Message Decode Failed");
        LOG_EXIT;
        return;
    } else if ( nasMsg.header.epd != _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
        higLog("Incorrect NAS Message type ");
        LOG_EXIT;
        return;
    } else if ( nasMsg.plain._5gmmMsg.mmheader.msgType != REGISTRATION_REJECT) {
        higLog("Incorrect NAS Message type ");
        LOG_EXIT;
        return;
    }

    RegistrationReject_t *regRej = &(nasMsg.plain._5gmmMsg.regReject);
    lowLog("Registraion Reject cause %d", regRej->_5gmmCause);
}

int handleUeSecurityParams(testUeContext_t &testUe)
{
	LOG_ENTRY;
	int rc = FAILURE;
	uint8_t ABBA = 0x00;//Refer to AMF, value kept same
	/* Below code is for actual computations of UE response & NF keys at AUSF & UDM
	and needs to be enabled once ausf & udm code integrated with NRF*/

	uint8_t SQN[SQN_SIZE]={}; //48 bit
	uint8_t AK[AK_SIZE]={}; //48 bit
	uint8_t IK[IK_SIZE]={};//128 bits
	uint8_t CK[CK_SIZE]={};//128 bits
	uint8_t RES[RES_SIZE]={}; //64 bits
	uint8_t Kausf[KAUSF_SIZE]={}; //256 bits
	uint8_t Kseaf[KSEAF_SIZE]={}; //256 bits

	char snName[4] = {};
	strcpy(snName, "amf");
	//Below value taken from UDM/same is taken as it is in cewit 5g_core udm_dev branch
	//Ultimately RAN emulator will need to fetch values from USIM and use for computations

	uint8_t KA[KA_SIZE] = {};
	uint8_t op[OP_SIZE] = {};
	memset(KA,0,KA_SIZE);
	memset(op,0,OP_SIZE);
	getLongTermKey(testUe.supi, KA);
	getOperatorArray(testUe.supi, op);

	//Enable below code to test with actual computed values from UDM
	//Step1: Calculate keys using f2,f3,f4,f5, ensure you get CORRECT value of RAND from AMF
	f2345(KA, op, testUe.secContext._5gAv.RAND, RES, AK, CK, IK);

	//Step3: Compute RES* from RES
	rc = computeXRES(testUe.secContext._5gAv.RAND, CK, IK, RES, snName, testUe.secContext._5gAv.RESstar);
	if(rc == FAILURE)
	{
		higLog("%s","computeXRES function failed!!");
		LOG_EXIT;
		return rc;
	}

	//Step3:Extract first 6 byes equivalent => SQN ^ AK
	uint8_t tempEXOR[SQN_SIZE]={};
	memcpy(tempEXOR , testUe.secContext._5gAv.AUTN, SQN_SIZE);
	int index=0;
	for(index=0; index<SQN_SIZE; index++)
		SQN[index]= tempEXOR[index] ^ AK[index];
	lowLog("%s","SQN:");
	printArray(SQN,SQN_SIZE);

	lowLog("%s","RES*:");
	printArray(testUe.secContext._5gAv.RESstar, XRES_SIZE);

	//Step4: Calculate Kausf using CK || IK as a key, SQN^AK||snName as input
	rc = computeKausf(CK,IK,SQN,AK,snName,Kausf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKausf failed!!");
		LOG_EXIT;
		return rc;
	}

	//Step5: Calculate Kseaf
	rc = computeKseaf(Kausf, snName, Kseaf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKseaf failed!");
		LOG_EXIT;
		return rc;
	}

	print_SUPI(testUe.supi);

	//Step6: Calculate Kamf
	//initializing Kamf precisely
	memset(testUe.Kamf,0,KAMF_SIZE);

	rc = computeKamf( Kseaf, testUe.supi, ABBA, testUe.Kamf);
	if(rc == FAILURE)
	{
		higLog("%s","computeKamf failed");
		LOG_EXIT;
		return rc;
	}

	//Step7: Calculate KNASint
	uint8_t algo_id = testUe.secContext.ueSecurityCapability._5gNASIntAlgo; //NAS_INTEGRITY_ALG_NIA2;
	uint8_t algo_distinguisher = N_NAS_INTEGRITY_ALG; //algo type distinguisher for N-NAS-int-algo
	rc = computeNasInt(testUe.Kamf, algo_distinguisher, algo_id, 
						testUe.secContext.K_NASint);
	if(rc == FAILURE)
	{
		higLog("%s","computation of NAS integrity key failed");
		LOG_EXIT;
		return rc;
	}
	lowLog("%s","K_NASint:");
	printHex(testUe.secContext.K_NASint, NAS_KEY_SIZE);
	//Step7: Calculate KNASenc
	algo_id = testUe.secContext.ueSecurityCapability._5gNASEncAlgo;//NAS_ENCRYPTION_ALG_NEA2
	algo_distinguisher = N_NAS_ENCRYPTION_ALG;//algo type distinguisher for N-NAS-enc-algo
	rc = computeNasEnc(testUe.Kamf, algo_distinguisher, algo_id,
		testUe.secContext.K_NASenc);
	if(rc == FAILURE)
	{
		higLog("Generation of K_NASint key failed!");
		LOG_EXIT;
		return rc;
	}

	LOG_EXIT;
	return SUCCESS;
}



void rcv_AMF_interactive(struct fdData *fdd)
{

	char msg[MAX_MESSAGE_SIZE] = {};
	NGAP_PDU_t  *ngapPdu = 0;
	asn_dec_rval_t decRet;
	int msgLen, rc = FAILURE;
	testUeContext_t testUe;

	if((msgLen = platformRecvData(fdd->listenFD, msg)) <= 0){
		higLog("%s","Failed to read from socket");
		exit(0);
	}

	/*decode msg into NGAP PDU*/
	decRet = ber_decode(0, &asn_DEF_NGAP_PDU,
					(void**) &ngapPdu, msg, msgLen);
	if(decRet.code == RC_OK) {
		lowLog("%s", "Decode succeeded");
		asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	} else {
		higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
						"RC_FAIL" : "RC_WMORE");
	}

	/* copy values into ue context*/
	testUe.ranUeNgapId = ngapGetRanUeNgapId(ngapPdu);
	testUe.amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);
	lowLog("testUe.ranUeNgapId: %d",testUe.ranUeNgapId);
	lowLog("testUe.amfUeNgapId: %d",testUe.amfUeNgapId);
	

	if((testUe.ranUeNgapId)!=-1){
		write(a[testUe.ranUeNgapId][1],&ngapPdu,sizeof(&ngapPdu));
	}
}




void rcv_AMF(struct fdData *fdd)
{

	while(1){
	char msg[MAX_MESSAGE_SIZE] = {};
	NGAP_PDU_t  *ngapPdu = 0;
	asn_dec_rval_t decRet;
	int msgLen, rc = FAILURE;
	testUeContext_t testUe;

	if((msgLen = platformRecvData(fdd->listenFD, msg)) <= 0){
		higLog("%s","Failed to read from socket");
		break;
	}

	/*decode msg into NGAP PDU*/
	decRet = ber_decode(0, &asn_DEF_NGAP_PDU,
					(void**) &ngapPdu, msg, msgLen);
	if(decRet.code == RC_OK) {
		lowLog("%s", "Decode succeeded");
		asn_fprint(stdout, &asn_DEF_NGAP_PDU, ngapPdu);
	} else {
		higLog("Decode failed, Err %s", decRet.code == RC_FAIL ?
						"RC_FAIL" : "RC_WMORE");
	}

	/* copy values into ue context*/
	testUe.ranUeNgapId = ngapGetRanUeNgapId(ngapPdu);
	testUe.amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);
	lowLog("testUe.ranUeNgapId: %d",testUe.ranUeNgapId);
	lowLog("testUe.amfUeNgapId: %d",testUe.amfUeNgapId);
	

	if((testUe.ranUeNgapId)!=-1){
		write(a[testUe.ranUeNgapId][1],&ngapPdu,sizeof(&ngapPdu));
	}
	}
}


int decodercv_AMF(NGAP_PDU_t  *ngapPdu)
{

	int rc = FAILURE;
	NAS_PDU_t *nasPdu = ngapGetNasPdu(ngapPdu);
	
	nasMessage_t nasMsg = {};
	rc = nasMessageDecode((uint8_t*) nasPdu->buf, &nasMsg, nasPdu->size, NULL);
	// printf("%d\n",nasMsg.plain._5gmmMsg.mmheader.msgType);
	if (rc==FAILURE) {
		higLog("NAS Message Decode Failed");
		ngapFree(ngapPdu);
		LOG_EXIT;
		return FAILURE;
	} else if ( nasMsg.header.epd != _5GS_MOBILITY_MANAGEMENT_MESSAGE) {
		higLog("Incorrect NAS Message type ");
		ngapFree(ngapPdu);
		LOG_EXIT;
		return FAILURE;
	} else if ( nasMsg.plain._5gmmMsg.mmheader.msgType != AUTHENTICATION_REQUEST){
		higLog("Incorrect NAS Message type ");
		ngapFree(ngapPdu);
		LOG_EXIT;
		return FAILURE;
	}

	ngapFree(ngapPdu);
	return SUCCESS;
}


void interactive(struct nfvInstanceData *ranNfvInst,int id)
{
	int choice;
	int flag=1;
	while(flag)
	{
		int rc = FAILURE;
		testUeContext_t testUe = {};
		initUeContext(testUe);
		testUe.ranUeNgapId = id;
		cout<<"Choose which message to send"<<endl;
		cout<<"1: NGsetup"<<endl;
		cout<<"2: Initial UE message"<<endl;
		cout<<"3: Authentication response"<<endl;
		cout<<"4: Security Mode complete"<<endl;
		cout<<"5: exit"<<endl;
		cin>>choice;
		switch(choice)
		{
			case 1:
				testNgSetupMessage(&ranNfvInst->fdData[E_AMF_INST_1]);
				testRecvNGSetupResponse(&ranNfvInst->fdData[E_AMF_INST_1]);
				break;
			case 2:
				{
				testSendInitialUeMessage(&ranNfvInst->fdData[E_AMF_INST_1] ,testUe);
				NGAP_PDU_t  *ngapPdu = 0;
				rcv_AMF_interactive(&ranNfvInst->fdData[E_AMF_INST_1]);
				read(a[id][0],&ngapPdu,sizeof(&ngapPdu));
				testUe.amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);			
				rc = decodercv_AMF(ngapPdu);
				}		
				break;
			case 3:
				{		
				testSendAuthResponse(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
				rcv_AMF_interactive(&ranNfvInst->fdData[E_AMF_INST_1]);
				NGAP_PDU_t  *ngapPdu = 0;
				read(a[id][0],&ngapPdu,sizeof(&ngapPdu));
				testUe.amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);			
				rc = decodercv_AMF(ngapPdu);
				}		
				break;
			case 4:
			{			
				testSendRegCompleteMsg(&ranNfvInst->fdData[E_AMF_INST_1], testUe);			
			}
				break;
			case 5:
				flag=0;
				break;
			default:
			cout<<"choose among given options"<<endl;
		}
	}
}


void procedure(struct nfvInstanceData *ranNfvInst,int id)
{
	while(UEflag){
		int rc = FAILURE;
		testUeContext_t testUe = {};
		initUeContext(testUe);
		testUe.ranUeNgapId = id;
		higLog("##############THREAD %d##############",id);
		testUe.ranUeNgapId = id;
		testSendInitialUeMessage(&ranNfvInst->fdData[E_AMF_INST_1] ,testUe);
		// rc = testRecvAuthReq(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
		higLog("##############THREAD %d##############",id);
		// rcv_AMF(&ranNfvInst->fdData[E_AMF_INST_1]);
		NGAP_PDU_t  *ngapPdu = 0;
		read(a[id][0],&ngapPdu,sizeof(&ngapPdu));
		testUe.amfUeNgapId = ngapGetAmfUeNgapId(ngapPdu);

		rc = decodercv_AMF(ngapPdu);
		mtx.lock();
		No_Of_Req++;
		mtx.unlock();
		higLog("##############THREAD %d##############",id);
		if(rc == SUCCESS) {
			handleUeSecurityParams(testUe);
			// sleep(1);
			testSendAuthResponse(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
		} else {
			lowLog("%s","ue has to retry\n");
			}
			// sleep(1);
		if(rc==SUCCESS){
			// rcv_AMF(&ranNfvInst->fdData[E_AMF_INST_1] );
			// NGAP_PDU_t  *ngapPdu = 0;
			read(a[id][0],&ngapPdu,sizeof(&ngapPdu));	
			higLog("##############THREAD %d##############",id);
			rc = decodercv_AMF(ngapPdu);
		}
		mtx.lock();
		No_Of_Req++;
		mtx.unlock();		
		// testRecvRegAccept(&ranNfvInst->fdData[E_AMF_INST_1]);
		// sleep(1);
		if(rc==SUCCESS){
			higLog("##############THREAD %d##############",id);
			testSendRegCompleteMsg(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
		}
	}
}



int main(int argc, char *argv[])
{
	
	struct nfvInstanceData *ranNfvInst;
	testUeContext_t testUe = {};
	int UE_Count;
	int timer;
	int choice;

	 //initialize all UE side parameters
	signal(SIGPIPE, sigpipe_handler);
	ranNfvInst = init(E_LOAD_GENERATOR_INST_1);

	/*TEST ONE:
	 * 1. Send a NgSetup Messages, process it's response.
	 * 2. Send a initial UE message to AMF & recv a response
	 * 3. Recv Authentication Request and save UE 5G Auth Vector.
	 * 4. If valid 5GAV is recvd, send a Authentication Response.
	 */
	cout<<"Choose the Modes"<<endl;
	cout<<"1: Interactive :One message at a time:"<<endl;
	cout<<"2: Procedural  :Multiple threads at once:"<<endl;
	cin>>choice;
	switch(choice)
	{
		case 1:
		{
			UE_Count = 1;
			for(int i=0; i<UE_Count;i++)
				if(pipe2(a[i],O_DIRECT )==-1)
					{
						perror("pipe");
						exit(1);
					}
			std::vector<std::thread> threads;
			for(int i=0; i<UE_Count;i++)
				{
					threads.push_back(std::thread(interactive,ranNfvInst,0));
				}
			for(auto& t: threads)
				t.join();
		}
		break;
		case 2:
		{
			cout<<"Number of UEs you want to emulate"<<endl;
			cin>>UE_Count;
			cout<<"How long you want emulator to run"<<endl;
			cin>>timer;
	
			testNgSetupMessage(&ranNfvInst->fdData[E_AMF_INST_1]);
			testRecvNGSetupResponse(&ranNfvInst->fdData[E_AMF_INST_1]);
			for(int i=0; i<UE_Count;i++)
			if(pipe2(a[i],O_DIRECT )==-1)
				{
					perror("pipe");
					exit(1);
				}

				std::vector<std::thread> threads;
				for(int i=0; i<UE_Count;i++)
				{
					threads.push_back(std::thread(procedure,ranNfvInst,i));
				}
			thread t1(rcv_AMF,&ranNfvInst->fdData[E_AMF_INST_1]);
			sleep(timer);
			UEflag=0;
			for(auto& t: threads)
				t.detach();
			t1.detach();
			printf("Request Processed: %ld\n",No_Of_Req);
		}
		break;
		default:
		cout<<"choose among give options"<<endl;	
	}




	
		

#if 0
	//Security mode control procedure
	rc = testRecvSecurityModeCommand(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
	if(rc == SUCCESS) {
		midLog("%s","SecurityModeCommand successfully decoded at UE!");
		midLog("%s","So sending Security mode Complete message to AMF...");
		sleep(2);
		testSendSecurityModeComplete(&ranNfvInst->fdData[E_AMF_INST_1], testUe);
	}
#endif


	

	successLog("=============================================================");
	successLog("=============  T E S T   O N E   C O M P L E T E  ===========");

	return 0;
}
