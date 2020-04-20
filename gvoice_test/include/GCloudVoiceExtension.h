/*******************************************************************************\
 ** gcloud_voice:GCloudVoiceExtension.h
 ** Created by CZ on 16/8/1
 **
 **  Copyright © 2016年 apollo. All rights reserved.
 \*******************************************************************************/

#ifndef gcloud_voice_GCloudVoiceExtension_h_
#define gcloud_voice_GCloudVoiceExtension_h_

#include "GCloudVoiceErrno.h"

namespace gcloud_voice
{
	#define DEVICE_DESCRIBLE_CHAR_NUM 256
	#define PARTICIPANT_ID_USERNAME 128
	#define PTT_STRING_NUM 64
	#define OPENID_LEN_MAX 128

	struct RoomMembers
	{
		int memberid ;
		char openid[OPENID_LEN_MAX];
	};
	enum DeviceType {
		RENDER_DEVICE = 0,
		CAPTURE_DEVICE = 1,		
			};

	struct Device_Info
	{
		char deviceid[DEVICE_DESCRIBLE_CHAR_NUM*2];
		char devicename[DEVICE_DESCRIBLE_CHAR_NUM*2];
	};
	struct L_Quality_Data
	{
		unsigned long incomingReceive;
		unsigned long incomingPacketloss;
		unsigned int maxLatency;
		unsigned int minLatency;
		L_Quality_Data()
		{
			incomingReceive = 0;
			incomingPacketloss = 0;
			maxLatency = 0;
			minLatency = 0;
		}
		void Reset()
		{
			incomingReceive = 0;
			incomingPacketloss = 0;
			maxLatency = 0;
			minLatency = 0;
		}
	};
    
    class IGCloudVoiceEngine;
    /**
     * Extension API for voice engine
     */
    class IGCloudVoiceEngineExtension
    {
    public:
        /**
         * Don't play voice of the member.
         * 
         * @param roomName: the room to join, should be less than 127byte, composed by alpha.
         * @param member : member to forbid
         * @param bEnable : do forbid if it is true
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno ForbidMemberVoice(int member, bool bEnable, const char *roomName="") = 0;
        
        /**
         * Open Voice Engine's logcat
         *
         * @param enable: open logcat if it is true
         */
        virtual void EnableLog(bool enable) = 0;
        
        /**
         * Get micphone's volume
         *
         * @return : micphone's volume , if return value>0, means you have said something capture by micphone
         */
        virtual int GetMicLevel(bool bFadeOut=true) = 0;
        
        /**
         * Get speaker's volume
         *
         * @return : speaker's volume, value is equal you Call SetSpeakerVolume param value
         */
        virtual int GetSpeakerLevel() = 0;

        /**
         * set sepaker's volume
         *
		 * @param vol: setspeakervolume, 
		 * Android & IOS, value range is 0-800, 100 means original voice volume, 50 means only 1/2 original voice volume, 200 means double original voice volume
		 * Windows value range is 0x0-0xFFFF, suggested value bigger than 0xff00, then you can hear you speaker sound
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno SetSpeakerVolume(int vol) = 0;
        
        /**
         * Test wheather microphone is available
         *
         * @return : if success return GCLOUD_VOICE_SUCC, , means have detect micphone device,failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno TestMic() = 0;
        
        /**
         * Get voice message's info
         * 
         * @param filepath: file path should be "sdcard/your_dir/your_file_name" be sure to use "/" not "\"
         * @param bytes: on return for file's size
         * @param sendons: on return for voice's length
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno GetFileParam(const char *filepath, unsigned int* bytes, float *seconds) = 0;
        
        /**
         * Capture microphone audio data by IGCloudVoiceNotify::OnRecording
         * 
         * @param bCapture : true/false - start/stop capturing audio data
         * @return : if success return GCLOUD_VOICE_SUCC, failed return other errno @see GCloudVoiceErrno
         * @see : GCloudVoiceErrno
         */
        virtual GCloudVoiceErrno CaptureMicrophoneData(bool bCapture) = 0;

        /*
        * Detect whether you're speaking or just keep microphone opened        
        * return bool value for the speaking state
        */
        virtual bool IsSpeaking()const = 0;

    public:
        /**
         * YOU SHOULD NOT INVOKE THIS ONE.
         */
        virtual int invoke ( unsigned int    nCmd,
                            unsigned int    nParam1,
                            unsigned int    nParam2,
                            unsigned int    *pOutput ) = 0;


		 /**
         * GetDevice Count number
         * 
         * @param devicetype : see DeviceType, capture or render
         * @return : device count number
         */
		virtual int GetDeviceCount(DeviceType devicetype) = 0;
		/**
         * Describle the index device
         * 
         * @param devicetype : see DeviceType, capture or render
		 * @param index : the index of device list, should small then GetDeviceCount return value
		 * @param Device_Info : the index device info
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno DescribleDevice(DeviceType devicetype, int index, Device_Info *info) = 0;
		/**
         * Set Is hear self voice
         * 
         * @param bEnable : true can hear self voice, false not hear self voice
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno EnableLoopRender(bool bEnable) = 0;
		/**
         * select device to used
         * 
         * @param devType : see DeviceType, capture or render
		 * @param strDevId : select device id
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno SelectDevice(DeviceType devType, char * strDevId) = 0;
		/**
         * get select device index, if not call Selectdevice, the pIndex will 0, then it will return the 
		 *	selectDevice deviceid index
         * 
         * @param devType : see DeviceType, capture or render
		 * @param pIndex : select device index
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno GetSelDevice(DeviceType devType, int* pIndex) = 0;

		/**
         * get user micvolume
         * 
         * @param pVol : [out] the suer micvolume value
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno GetMicVolume(unsigned int *pVol) = 0;
		/**
         * get user speaker volume
         * 
         * @param pVol : [out] the suer speaker volume value
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno GetSpeakerVolume(unsigned int *pVol) = 0;
		/**
         * get is loopback user sound now
         * 
         * @return : true: loopback user sound now, false: not loopback user sound
         */
		virtual bool IsEnableLoopRender() = 0;

		/**
         * get room member's volume
         * 
		 * @param memid: the room member's memberid
         * @param pVol : [out] the member's volume, default is 100
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno GetMemVol(long memid, unsigned int *pVol) = 0;
		/**
         * set room member's volume
         * 
		 * @param memid: the room member's memberid
         * @param pVol : [in] the member's volume
         * @return : see GCloudVoiceErrno
         */
		virtual bool SetMemVol(long memid, unsigned int nVol) = 0;
		/**
         * get is the room member's is be mute
         * 
		 * @param memid: the room member's memberid
         * @return : if the membervoice be forbid,return true(means can't hear this memberid voice), else return false
         */
		virtual bool IsMemberBeForbidVoice(unsigned int memid) = 0;
		/**
         * not be used now, get the communication quality data
         * 
         * @param L_Quality_Data : [out] the comunication qualitydata
         * @return : see GCloudVoiceErrno
         */
		virtual GCloudVoiceErrno LI_GetQualityData(L_Quality_Data* pData) = 0;
		/**
         * get all room members
         * @param roomname: the roomname
         * @param members : [out] the members array
		 * @param members : [in] the array len
         * @return : the member's num
         */
		virtual int GetRoomMembers(const char* roomName, RoomMembers members[], int len) = 0;
		/**
         * set voice vad sensitive, 0 is not sensitive, more voice data will be send, and more net traffic need
         * @param mode: the vad sensitive value, range[0,100]
         * @return : true:set success, false: set vad sensitive fail
         */
		virtual bool SetVadSensitive(int mode) = 0;
		/**
         * is mic open or close now
         * @return : true:mic have open, false: mic is closed
         */
		virtual bool IsMicEnabled() = 0;
		/**
         * is speaker open or close now
		 * @return : true:speaker have open, false: speaker is closed
         */
		virtual bool IsSpeakerEnabled() = 0;
		/**
         * reserved
         */
		virtual void IsRspJoinRoomBroadcast(bool enable) = 0;
		/**
         * get vad sensitive value
		 * @return : the vad sensitive value
         */
		virtual int GetVadSensitive() = 0;

		virtual void DestroyVoiceEngine() = 0;
	};
} // end of gcloud_voice
  

#endif /* gcloud_voice_GCloudVoiceExtension_h_ */
