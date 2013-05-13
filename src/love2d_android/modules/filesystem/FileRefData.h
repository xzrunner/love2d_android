// #ifndef LOVE_FILESYSTEM_FILE_REF_DATA_H
// #define LOVE_FILESYSTEM_FILE_REF_DATA_H
// 
// // STD
// #include <string>
// 
// // LOVE
// #include <common/Data.h>
// #include <common/int.h>
// 
// namespace love
// {
// namespace filesystem
// {
// 	class FileRefData : public Data
// 	{
// 	private:
// 
// 		// The actual data.
// 		const void* data;
// 
// 		// Size of the data.
// 		uint64 size;
// 
// 		// The filename used for error purposes.
// 		std::string filename;
// 
// 		// The extension (without dot). Used to identify file type.
// 		std::string extension;
// 
// 	public:
// 
// 		FileRefData(const void* data, uint64 size, const std::string& filename);
// 
// 		virtual ~FileRefData();
// 
// 		// Implements Data.
// 		void * getData() const;
// 		//TODO: Enable this
// 		//uint64 getSize() const;
// 		int getSize() const;
// 
// 		const std::string & getFilename() const;
// 		const std::string & getExtension() const;
// 
// 	}; // FileRefData
// 
// } // filesystem
// } // love
// 
// #endif // LOVE_FILESYSTEM_FILE_REF_DATA_H
