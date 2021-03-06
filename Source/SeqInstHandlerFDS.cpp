/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** 0CC-FamiTracker is (C) 2014-2018 HertzDevil
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

#include "SeqInstHandlerFDS.h"
#include "InstrumentFDS.h"
#include "ChannelHandlerInterface.h"

/*
 * Class CSeqInstHandlerFDS
 */

void CSeqInstHandlerFDS::LoadInstrument(std::shared_ptr<CInstrument> pInst)		// // //
{
	CSeqInstHandler::LoadInstrument(pInst);

	if (auto pFDSInst = std::dynamic_pointer_cast<const CInstrumentFDS>(m_pInstrument))
		UpdateTables(*pFDSInst);
}

void CSeqInstHandlerFDS::TriggerInstrument()
{
	CSeqInstHandler::TriggerInstrument();

	CChannelHandlerInterfaceFDS *pInterface = dynamic_cast<CChannelHandlerInterfaceFDS*>(m_pInterface);
	if (pInterface == nullptr) return;
	auto pFDSInst = std::dynamic_pointer_cast<const CInstrumentFDS>(m_pInstrument);
	if (pFDSInst == nullptr) return;
	pInterface->SetFMSpeed(pFDSInst->GetModulationSpeed());
	pInterface->SetFMDepth(pFDSInst->GetModulationDepth());
	pInterface->SetFMDelay(pFDSInst->GetModulationDelay());
	UpdateTables(*pFDSInst);
}

void CSeqInstHandlerFDS::UpdateInstrument()
{
	CSeqInstHandler::UpdateInstrument();

	if (dynamic_cast<CChannelHandlerInterfaceFDS*>(m_pInterface))
		if (auto pFDSInst = std::dynamic_pointer_cast<const CInstrumentFDS>(m_pInstrument))
			UpdateTables(*pFDSInst);
}

void CSeqInstHandlerFDS::UpdateTables(const CInstrumentFDS &Inst)
{
	if (auto pInterface = dynamic_cast<CChannelHandlerInterfaceFDS*>(m_pInterface)) {
		pInterface->FillWaveRAM(Inst.GetSamples());
		pInterface->FillModulationTable(Inst.GetModTable());
	}
}
