#include "storage\sector-manager.hpp"

#include <cstdlib>

#define NUMBER_OF_SECTORS 10

SectorManager::SectorManager() {
	allocation_table_ = new Bitmap(NUMBER_OF_SECTORS, false);
	storage_pointer_ = (int*)malloc(NUMBER_OF_SECTORS * SECTOR_MAX_SIZE);
	sector_size_ = SECTOR_MAX_SIZE;
	last_used_ = 0;
}

ErrorCode SectorManager::allocate_sector(SectorID * sector_id){
	//Starts to search for the desired empty space from the previous successful one
	(*sector_id) = (*allocation_table_).give_free_bit(last_used_);
	last_used_ = (*sector_id);

	//If the table is full, no free sector can be returned
	if (*sector_id == -1)
		return ErrorManager::error(__HERE__, ER_ALLOCATION_TABLE_FULL);

	//Returns the id of the allocated sector
	return NO_ERROR;
}

ErrorCode SectorManager::deallocate_sector(SectorID sector_id) {
	
	//Check if sector number is in range
	if (sector_id < 0 || sector_id >= NUMBER_OF_SECTORS)
		return ErrorManager::error(__HERE__, ER_SECTOR_OUT_OF_RANGE, sector_id,NUMBER_OF_SECTORS-1);

	//Check if sector is actually allocated
	if (!(*allocation_table_).is_bit_set(sector_id)) {
		return ErrorManager::error(__HERE__, ER_SECTOR_NOT_USED, sector_id);
	}
	else {
		(*allocation_table_).clear_bit(sector_id);
		return NO_ERROR;
	}

}

bool SectorManager::is_sector_allocated(SectorID sector_id) {
	return (*allocation_table_).is_bit_set(sector_id);
}

int* SectorManager::get_sector_pointer(SectorID sector_id) {
	if (sector_id >= 0 && sector_id < NUMBER_OF_SECTORS)
		return storage_pointer_ + sector_id*sector_size_;
}
