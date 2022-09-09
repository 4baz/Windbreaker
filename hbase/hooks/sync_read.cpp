#include "../hooking.h"
#include"../natives.h"

namespace hbase
{
	bool hooks::sync_read(rage::netSyncTree* netSyncTree, int32_t sync_type, int32_t _sync_flag, rage::datBitBuffer* buffer, void* netLogStub)
	{
		auto ret = g_hooking->m_sync_read_hook.get_original<functions::sync_read_t>()(netSyncTree, sync_type, _sync_flag, buffer, netLogStub);
		//Hash mHash = 0;
		//__int64 pCEntityCreationDataNode = NULL;
		//for (int i = 0; i < 13; i++)
		//{
		//	if (g_pointers->m_get_sync_tree_from_type(NULL, i) == netSyncTree)
		//	{
		//		switch (i) // i = syncTreeId
		//		{
		//		case 5:  // CObjectSyncTree

		//			pCEntityCreationDataNode = *(__int64*)((unsigned __int64)netSyncTree + 48);
		//			mHash = *(Hash*)(pCEntityCreationDataNode + 336);

		//			//ÎÞÐ§
		//			if (!STREAMING::IS_MODEL_VALID(mHash))
		//			{
		//				*(Hash*)(pCEntityCreationDataNode + 336) = rage::joaat("prop_paints_can07");
		//			}
		//			break;

		//			//Áý×Ó
		//			switch (mHash)
		//			{
		//			case 379820688: // prop_dog_cage_01
		//			case 1692612370: // prop_dog_cage_02
		//			case 959275690: // prop_gold_cont_01
		//			case 1396140175: // prop_gold_cont_01b

		//				break;
		//				*(Hash*)(pCEntityCreationDataNode + 336) = rage::joaat("prop_paints_can07");
		//			}

		//			break;

		//		case 11:  // CPlayerSyncTree
		//			//if (CMitigationMgr::bLogSyncTree)
		//				//cmd::write(GRAY, "CPlayerSyncTree: %p", pNetSyncTree);
		//			break;

		//		case 0:  // CAutomobileSyncTree
		//		case 1:	 // CBikeSyncTree
		//		case 2:	 // CBoatSyncTree
		//		case 4:  // CHeliSyncTree
		//		case 9:  // CPlaneSyncTree
		//		case 12: // CAutomobileSyncTree
		//		case 13: // CTrainSyncTree

		//			//if (CMitigationMgr::bLogSyncTree)
		//				//cmd::write(GRAY, "CVehicleSyncTree: %p", pNetSyncTree);

		//			pCEntityCreationDataNode = *(__int64*)((unsigned __int64)netSyncTree + 48);
		//			mHash = *(Hash*)(pCEntityCreationDataNode + 200);
		//			//hbase::settings::lastSyncModelHashReceived = mHash;

		//			//patch the vehicle hash if its invalid
		//			if (!STREAMING::IS_MODEL_VALID(mHash))
		//			{
		//				*(Hash*)(pCEntityCreationDataNode + 200) = rage::joaat("comet");
		//			}
		//			break;
		//		}
		//	}
		//}

		return  ret;
	}
}