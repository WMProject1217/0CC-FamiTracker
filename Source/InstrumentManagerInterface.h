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


#pragma once

#include <memory>

class CSequence;
class CSeqInstrument;

enum inst_type_t : unsigned;
enum class sequence_t : unsigned;

namespace ft0cc::doc {
class dpcm_sample;
} // namespace ft0cc::doc

/*
	\brief Interface for instrument resource containers. An instance is injected into each
	instrument belonging to a collection.
*/
class CInstrumentManagerInterface
{
public:
	/*!	\brief Obtains a sequence resource.
		\param InstType The instrument type, which should be a member of inst_type_t.
		\param SeqType The sequence type.
		\param Index The sequence index.
		\return Pointer to the sequence. */
	virtual std::shared_ptr<CSequence> GetSequence(inst_type_t InstType, sequence_t SeqType, int Index) const = 0;
	/*!	\brief Puts a sequence into the resource container.
		\param InstType The instrument type, which should be a member of inst_type_t.
		\param SeqType The sequence type.
		\param Index The sequence index.
		\param pSeq Pointer to the sequence. */
	virtual void SetSequence(inst_type_t InstType, sequence_t SeqType, int Index, std::shared_ptr<CSequence> pSeq) = 0;
	/*!	\brief Adds a sequence into the resource container.
		\param InstType The instrument type, which should be a member of inst_type_t.
		\param SeqType The sequence type.
		\param pSeq Pointer to the sequence.
		\param pInst Pointer to the current instrument, required to return the correct sequence index.
		\return The index of the sequence, or -1 if it is not inserted. */
	virtual int AddSequence(inst_type_t InstType, sequence_t SeqType, std::shared_ptr<CSequence> pSeq, CSeqInstrument *pInst) = 0;
	/*!	\brief Accesses a DPCM sample resource.
		\param Index The sample index.
		\return Pointer to the sample. */
	virtual std::shared_ptr<ft0cc::doc::dpcm_sample> GetDSample(int Index) = 0;
	/*!	\brief Accesses a DPCM sample resource.
		\param Index The sample index.
		\return Pointer to the sample. */
	virtual std::shared_ptr<const ft0cc::doc::dpcm_sample> GetDSample(int Index) const = 0;
	/*!	\brief Puts a DPCM sample into the resource container.
		\param Index The sample index.
		\param pSamp Pointer to the sample. */
	virtual void SetDSample(int Index, std::shared_ptr<ft0cc::doc::dpcm_sample> pSamp) = 0;
	/*!	\brief Adds a DPCM sample into the resource container.
		\param pSamp Pointer to the sample.
		\return The index of the sample, or -1 if it is not inserted. */
	virtual int AddDSample(std::shared_ptr<ft0cc::doc::dpcm_sample> pSamp) = 0;
};
