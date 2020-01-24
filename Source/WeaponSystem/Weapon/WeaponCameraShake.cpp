// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCameraShake.h"

UWeaponCameraShake::UWeaponCameraShake()
{
	//how long the camera wobbles for
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.0f;
	OscillationBlendOutTime = 0.15f;

	//in which direction we shake the camera and frequency of how often it shakes
	RotOscillation.Pitch.Amplitude = 0.1f;
	RotOscillation.Pitch.Frequency = 10.f;
	RotOscillation.Yaw.Amplitude = 0.f;
	RotOscillation.Yaw.Frequency = 0.f;


}
