/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2015 HertzDevil
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

// This file handles playing of VRC6 channels

#include "stdafx.h"
#include "FamiTracker.h"
#include "FamiTrackerDoc.h"
#include "ChannelHandler.h"
#include "ChannelsVRC6.h"
#include "SoundGen.h"

CChannelHandlerVRC6::CChannelHandlerVRC6() : CChannelHandler(0xFFF, 0x0F)
{
}

void CChannelHandlerVRC6::HandleCustomEffects(int EffNum, int EffParam)
{
	if (!CheckCommonEffects(EffNum, EffParam)) {
		switch (EffNum) {
			case EF_DUTY_CYCLE:
				m_iDefaultDuty = m_iDutyPeriod = EffParam;
				break;
			// // //
		}
	}
}

bool CChannelHandlerVRC6::HandleInstrument(int Instrument, bool Trigger, bool NewInstrument)
{
	CFamiTrackerDoc *pDocument = m_pSoundGen->GetDocument();
	CInstrumentContainer<CSeqInstrument> instContainer(pDocument, Instrument);		// // //
	CSeqInstrument *pInstrument = instContainer();

	if (pInstrument == NULL)
		return false;

	m_iInstTypeCurrent = pInstrument->GetType();		// // //
	for (int i = 0; i < SEQ_COUNT; ++i) {
		const CSequence *pSequence = pDocument->GetSequence(pInstrument->GetType(), pInstrument->GetSeqIndex(i), i); // // //
		if (Trigger || !IsSequenceEqual(i, pSequence) || pInstrument->GetSeqEnable(i) > GetSequenceState(i)) {
			if (pInstrument->GetSeqEnable(i) == 1)
				SetupSequence(i, pSequence);
			else
				ClearSequence(i);
		}
	}

	return true;
}

void CChannelHandlerVRC6::HandleEmptyNote()
{
}

void CChannelHandlerVRC6::HandleCut()
{
	CutNote();
}

void CChannelHandlerVRC6::HandleRelease()
{
	if (!m_bRelease) {
		ReleaseNote();
		ReleaseSequences();
	}
}

void CChannelHandlerVRC6::HandleNote(int Note, int Octave)
{
	m_iNote		  = RunNote(Octave, Note);
	m_iSeqVolume  = 0x0F;
	m_iDutyPeriod = m_iDefaultDuty;
}

void CChannelHandlerVRC6::ProcessChannel()
{
	CFamiTrackerDoc *pDocument = m_pSoundGen->GetDocument();

	// Default effects
	CChannelHandler::ProcessChannel();

	// Sequences
	for (int i = 0; i < SEQ_COUNT; ++i)
		RunSequence(i);
}

void CChannelHandlerVRC6::ResetChannel()
{
	CChannelHandler::ResetChannel();
}

void CChannelHandlerVRC6::ClearRegisters()		// // //
{
	uint16 Address = ((m_iChannelID - CHANID_VRC6_PULSE1) << 12) + 0x9000;
	WriteExternalRegister(Address, 0);
	WriteExternalRegister(Address + 1, 0);
	WriteExternalRegister(Address + 2, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// // // VRC6 Squares
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVRC6Square::RefreshChannel()
{
	uint16 Address = ((m_iChannelID - CHANID_VRC6_PULSE1) << 12) + 0x9000;

	unsigned int Period = CalculatePeriod();
	unsigned int Volume = CalculateVolume();
	unsigned char DutyCycle = m_iDutyPeriod << 4;

	unsigned char HiFreq = (Period & 0xFF);
	unsigned char LoFreq = (Period >> 8);
	
	if (!m_bGate) {		// // //
		WriteExternalRegister(Address, DutyCycle);
		return;
	}

	WriteExternalRegister(Address, DutyCycle | Volume);
	WriteExternalRegister(Address + 1, HiFreq);
	WriteExternalRegister(Address + 2, 0x80 | LoFreq);
}

int CVRC6Square::ConvertDuty(int Duty) const		// // //
{
	switch (m_iInstTypeCurrent) {
	case INST_2A03:	return DUTY_VRC6_FROM_2A03[Duty & 0x03];
	case INST_N163:	return Duty;
	case INST_S5B:	return 0x07;
	default:		return Duty;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// VRC6 Sawtooth
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVRC6Sawtooth::RefreshChannel()
{
	unsigned int Period = CalculatePeriod();

	unsigned char HiFreq = (Period & 0xFF);
	unsigned char LoFreq = (Period >> 8);

	int Volume = (CalculateVolume() << 1) | ((m_iDutyPeriod & 1) << 5);		// // //

	if (Volume < 0)
		Volume = 0;
	if (Volume > 63)
		Volume = 63;
	
	if (!m_bGate) {		// // //
		WriteExternalRegister(0xB000, 0);
		return;
	}

	WriteExternalRegister(0xB000, Volume);
	WriteExternalRegister(0xB001, HiFreq);
	WriteExternalRegister(0xB002, 0x80 | LoFreq);
}
