#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
// #include <stdlib.h>

	#if (!defined(CSENSE_V1) && !defined(CSENSE_V2))
		#error Something is wrong!
	#endif

	/**		!!!	WARNINGS  !!!
			@brief:	In order to enter TEST MODE ,the code should be compiled in RELEASE mode and related pin out should be shorted.			
	*/

	//		#define RAW_BUFFER_OUTPUT								/*	SET THE HEAP SIZE 0x8000   Heap_Size       EQU     0x00008000			*/						

	/*	----------------SYSTEM DEFINITIONS -----------------		*/
			#ifdef CSENSE_V1
					#define		CoreFrequency							(uint32_t)72000						//Khz [SAFE : 72Mhz UNSAFE 120Mhz] because of DAC resolution MIN should be 24K
			#else
					#define		CoreFrequency							(uint32_t)32000						//Khz [SAFE : 72Mhz UNSAFE 120Mhz] because of DAC resolution MIN should be 24K [...24-32-40-48-56-64-72-80-88-96-104-112-120...]
			#endif
			
			#define debugBuffer											(uint16_t)250				// byte
			#define uartBaudrate										(uint32_t)1000000		// bps
			#define ADCVoltageREF										(float)3.0F					// Volts			/* Used together with (ADC1->CFGR>>3)&0x0F to revert back the ADC Value into the voltage value */
				
	//WATCHDOG		----------------------------------------------------------------------------------------->
			#define	WATCHDOG_STATUS									 true
			#define	WATCHDOG_LENGTH									(float)0.500F		//Second

								
			// #define _enable_algo_tx_								  // overrides by __DEBUG__
			#define _EnableDetectionCore_							// my test output with detection core switch

	//FOR DAC						----------------------------------------------------------------------------------->
			#define 	sinLength 										(uint16_t)64			// DAC UPDATE FREQUENCY IS MAX  1 MS/sec therefore we can use max 64 steps of buffer in order to create 15625 Hertz frequency
			/**				sinLength CoreFrequency relation
								<=70			24000
								<=96			32000
								<=120			40000
								<=160			48000
								<=xxx			56000
								.....
			*/
			
			#define 	dacMinVoltageLevel							(float)0.5F			// DON'T CHANGE this value. Unless you know what you do!
			#define		dacOffsetVoltage								(float)1800.0F	// mVolt
	//FOR RGB						----------------------------------------------------------------------------------->
			#define 	RGBCommon_Brigthness						(uint8_t)20				// percent max %100
			
			#ifdef CSENSE_V1
				#define 	NEW_RGB_SELECTED							false							// IN-P55TATRGB NEW (true) --- SLV6A-FKB FORMER (false)
			#else
				#define 	NEW_RGB_SELECTED							false							// IN-P55TATRGB NEW (true) --- SLV6A-FKB FORMER (false)
			#endif

	/* Because of the rgb scheduler is removed and put under LCD part, control time is increased therefor transition became flickered */
	//	#define 	RGBTransitionEnable_ 					
			#ifdef 		RGBTransitionEnable_
					#define 	RGBTransitionStartDelay				(uint8_t)2			// in Seconds
			#endif

//FOR	EASTEREGG					----------------------------------------------------------------------------------->			
			#define 	ActivateEasterEgg								true
			
			
//FOR	BATTERY ICON			----------------------------------------------------------------------------------->		

	#ifdef CSENSE_V2
		/* this locks the battery bar at 1 bar after depleting below 2bar. it avoids the battery icon go further back upto 2 bar, when the battery recover itself back on */
			#define		BatteryLineLockActivate				true	
			#define		InitMinimumLevelCheck					true
			#define		FilterDiffActivation					true		/* calculates the battery health using slow and fast filter difference */
					#define 	TWOBarBatteryDiffs				(uint8_t)75
					#define 	ONEBarBatteryDiffs				(uint8_t)150
					#define		NOBarBatterDiffs					(uint8_t)225					
	#endif			
	
//FOR	SCHEDULER			----------------------------------------------------------------------------------->
				/*		TIM2
						masterTimerResolution value effects the dac signal frequency directly, however sampling relation doesn't changed.
						Moreover also effects the battery sampling frequency because master Timer ,which is TIM2, drives three timers synchonous including battery trigger timer TIM 8
				*/
				#define masterTimerResolution			(const uint32_t) (2000000)			// Hertz,
				
				#ifdef CSENSE_V2
					#define samplingCountPerCycle			(uint8_t)4										/* 8 and 16 point of samplings processor speed should be made 40Mhz */
				#else	
					#define samplingCountPerCycle			(uint8_t)4										// more than this number increases the calculation on quadrature assignment
				#endif
				
				/* 		TIM8
						Battery sampling frequency should be set to 250 hertz in order to get the filter answer of 30 second			 
						if it is changed, relative to this you can get a higher or lower filter answer. 	
				*/
				#define batterySamplingFrequency	(const uint16_t)250					// may be set as low as 50 hertz, consider battery empty time filter answer.Optimum is 250.
				
				/*	it should work with any value entered, however, algorithm timestep is 1mSec and therefore should be multiples of this value
						also the filter used to smooth the signal should be considered before entering a different value,because higher values delays the filters answer.
						Around 4mSec is fine.
				*/
				#define schedulerTimeValue				(uint16_t)4000 												//uSec, resolution	is (64uSec) --- should be 4000 --- 
				
				
				/*		Because of the system limitation (in algorithm), integrationTime lower 8n causes to compiler optimized out the process section	*/				
				#define	integrationTimeValue			(uint16_t)(8*schedulerTimeValue)			// n times of scheduler in usec -shouldn't be changed because code can't handle the difference for now.-
				
//FOR COIL VOLTAGE	----------------------------------------------------------------------------------->

				extern double 	CoilVoltage;
				
				#define 	CSV1_CoilVoltage				 (float)60.0F			// min: 5V - max : 91 Volts,above opamp voltage is getting close to rail, calibration required to reach this level
																														// because of shielding cable, at 5Volts is gives out "TX cable cut error" 10 is OK
				#define 	CSV2_CoilVoltage				 (float)80.0F			// max:120 Volt
				#define		ActivateLowTXWarning			true
					
// PATCH
				
				/*	calibration patch disables the seg5 pinout because toggling of this pin injects noise into the system.
						the noise injected doesn't have a direct impact on RX channel, therefore left untouched. However,
						while magnitute calibration, noise should be considered. Therefore in the time of calibration, the pin is disabled 
						and after the calibration it is enabled again.
				*/
				#ifdef CSENSE_V1
						#define 	applyCalibrationPatch					true
				#else
						#define 	applyCalibrationPatch					false
				#endif

				
// TEST CODE RELATED				-------------------------------------------------------------------------->
	#define test_beep_freq																				(uint16_t)2500		//Hertz
	#define test_beep_duty																				(uint8_t)1				
	#define test_beep_length																			(float)(0.5F)
	#define test_between_stages_button_pressed_length							(uint16_t)((0.3F)/(schedulerTimeValue/1000000.0F))		//sec
						
	#ifdef CSENSE_V1
			#define MAX_TestCount		5
	#else
			#define MAX_TestCount		5
	#endif
						
// BATTERY RELATED					-------------------------------------------------------------------------->
	/*
			Fast filter action is added in order to catch the battery value dropping and controllable turn off the device.
			To activate this part set the activatefastFilterResponse as true
	*/
	#define activatefastFilterResponse														true
	
// VERSION RELATED					-------------------------------------------------------------------------->	
	
	#define CSenseVersion2		(char)0xF0
	#define CSenseVersion1		(char)0x0F
	#define CSV2MemoryAddress	(uint32_t)0x08014000				// MUST BE CHANGED IN THE SCATTER FILE ALSO !!!!
	
	//"This only works for arrays on the stack, e.g. if you're using malloc() or accessing a function parameter, you're out of luck.."
	//@ https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
	#define getSizeArray(x)  (sizeof(x) / sizeof((x)[0]))
	
				 void 		VolumeSet(bool state);
	extern char			t_version_checked;
	
		#ifdef RAW_BUFFER_OUTPUT
			#ifdef debugBuffer
					#undef 		debugBuffer
					#undef 		CoreFrequency
			#endif
	
			#define		CoreFrequency																					72000
			#define 	debugBuffer																						25000			// byte			
	
			#warning 	"Update Heap Relatively"
		#endif
	
	
#endif

