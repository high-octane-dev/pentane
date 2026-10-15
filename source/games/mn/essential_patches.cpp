#include "../../sunset/sunset.hpp"
#include "essential_patches.hpp"

struct VNObject {
	char reserved[12];
};

class SerializeableInterface {
	struct {
		int(*__thiscall Serialize)(SerializeableInterface*, void*, int);
		int(*__thiscall DeSerialize)(SerializeableInterface*, void*, int);
		int(*__thiscall GetSerializedDataSize)(SerializeableInterface*);
	}* vt;
};

template <typename T> struct ContainerList {
public:
	unsigned int length;
	unsigned char flags;
	T* memory;
	unsigned int capacity_add;
	unsigned int capacity;
	unsigned int max;
};

struct Record {
	char reserved[0x30];
};

struct CarsRecordSet {
	void** vt;
	void* event_info;
	ContainerList<Record> records;
	void* record_library;
};

struct CarsRecordLibrary {
	VNObject base;
	SerializeableInterface serializer;
	ContainerList<CarsRecordSet*> record_sets;
	ContainerList<char*> profile_names;
	unsigned int unknown;
	unsigned int time_stamp;
	unsigned int unknown2;
};

/*
Ensures that the streaming_package_index is valid, preventing crashes when jumping to join points.
(Only occurs when the scene is streamed.)
*/
DefineReplacementHook(StreamingManager_BlockUntilAllSectionsAreReady) {
	static void __fastcall callback(struct StreamingManager* _this, uintptr_t edx, int streaming_package_index) {
		if (streaming_package_index != -1) {
			original(_this, edx, streaming_package_index);
		}
	}
};

/*
Properly implements the logic for determining the size of CarsRecordLibrary when serialized.
In the original game, they just create a scratch buffer and try serializing, which is problematic when its final size ends up exceeding the size of the scratch buffer.
*/
DefineReplacementHook(FixCarsRecordLibrary) {
	static int __fastcall callback(std::uintptr_t ecx) {
		CarsRecordLibrary* _this = reinterpret_cast<CarsRecordLibrary*>(ecx - sizeof(VNObject));
		// size = 2 * sizeof(int) + profile_name_count * sizeof(char[32])
		int size = 8 + _this->profile_names.length * 32;
		for (int i = 0; i < _this->record_sets.length; i++) {
			int record_count = _this->record_sets.memory[i]->records.length;
			// record_set_size = record_count * sizeof(Record) + sizeof(int) + sizeof(char[64])
			int record_set_size = record_count * 28 + 4 + 64;
			size += record_set_size;
		}
		return size;
	}
};

/*
Patches `X360LoadingIcon::Enable` to choose the correct loading screen texture instead of hardcoding `load_logo` every time.
*/
DefineInlineHook(FixLoadingScreenPath) {
	static void callback(sunset::InlineCtx & ctx) {
		std::string calculated_path{};
		if (*reinterpret_cast<std::uint32_t*>(0x006F72CC) == 2) {
			calculated_path = std::format("{}\\ui\\tex_hd\\{}", *reinterpret_cast<char*>(0x006f6460), reinterpret_cast<char*>(*reinterpret_cast<std::uintptr_t*>(0x00718a74) + 0x764));
		}
		else {
			calculated_path = std::format("{}\\ui\\tex\\{}", *reinterpret_cast<char*>(0x006f6460), reinterpret_cast<char*>(*reinterpret_cast<std::uintptr_t*>(0x00718a74) + 0x764));
		}
		if (calculated_path.size() < 0x104) {
			char* buffer = *reinterpret_cast<char**>(ctx.esp.pointer);
			std::memset(buffer, 0, 0x104);
			std::memcpy(buffer, calculated_path.data(), calculated_path.size());
		}
	}
};


auto essential::mn::install() -> void {
	// Prevents crashes when jumping to a join point.
	StreamingManager_BlockUntilAllSectionsAreReady::install_at_ptr(0x005b66e0);

	// Prevents crashes when too many events are added to the global database.
	FixCarsRecordLibrary::install_at_ptr(0x0048e580);

	// Restores the `LoadingScreenName` functionality in `SceneInfo.dat`.
	FixLoadingScreenPath::install_at_ptr(0x004053a7);

	// Fixes an issue where fog colors would be erroneously divided by 4.
	sunset::utils::Access restore = sunset::utils::set_permission(reinterpret_cast<double*>(0x00679198), sizeof(double), sunset::utils::Access::ReadWrite).unwrap();
	*reinterpret_cast<double*>(0x00679198) = 255.0f;
	sunset::utils::set_permission(reinterpret_cast<double*>(0x00679198), sizeof(double), restore).unwrap();
}