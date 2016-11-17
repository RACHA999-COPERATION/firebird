/*
 *	PROGRAM:		JRD access method
 *	MODULE:			Mapping.h
 *	DESCRIPTION:	Maps names in authentication block
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2014 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef JRD_MAPPING
#define JRD_MAPPING

#include "../common/classes/alloc.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/ClumpletReader.h"
#include "../jrd/recsrc/RecordSource.h"
#include "../jrd/Monitoring.h"
#include "../jrd/scl.h"

namespace Jrd {

ULONG mapUser(const bool throwNotFoundError,
	Firebird::string& name, Firebird::string& trusted_role, Firebird::string* auth_method,
	Firebird::AuthReader::AuthBlock* newAuthBlock, UserId::Privileges* system_privileges,
	const Firebird::AuthReader::AuthBlock& authBlock, const char* alias, const char* db,
	const char* securityDb, const Firebird::string& sql_role,
	Firebird::ICryptKeyCallback* cryptCb, Firebird::IAttachment* att);
// bits returned by mapUser
const ULONG MAPUSER_ERROR_NOT_THROWN = 1;
const ULONG MAPUSER_MAP_DOWN = 2;

void clearMappingCache(const char* dbName, USHORT index);
// possible index values
const USHORT MAPPING_CACHE = 0;
const USHORT SYSTEM_PRIVILEGES_CACHE = 1;

void shutdownMappingIpc();

class GlobalMappingScan: public VirtualTableScan
{
public:
	GlobalMappingScan(CompilerScratch* csb, const Firebird::string& alias,
					  StreamType stream, jrd_rel* relation)
		: VirtualTableScan(csb, alias, stream, relation)
	{}

protected:
	const Format* getFormat(thread_db* tdbb, jrd_rel* relation) const override;
	bool retrieveRecord(thread_db* tdbb, jrd_rel* relation, FB_UINT64 position,
		Record* record) const override;
};

class MappingList : public SnapshotData
{
public:
	explicit MappingList(jrd_tra* tra);

	RecordBuffer* getList(thread_db* tdbb, jrd_rel* relation);

private:
	RecordBuffer* makeBuffer(thread_db* tdbb);
};

} // namespace Jrd


#endif // JRD_MAPPING
