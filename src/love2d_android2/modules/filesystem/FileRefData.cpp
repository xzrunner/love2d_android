// #include "FileRefData.h"
// 
// // STD
// #include <algorithm>
// 
// namespace love
// {
// namespace filesystem
// {
// 	FileRefData::FileRefData(const void* data, uint64 size, const std::string& filename)
// 		: data(data)
// 		, size(size)
// 		, filename(filename)
// 	{
// 		std::transform(this->filename.begin(), this->filename.end(), this->filename.begin(), tolower);
// 		if (this->filename.rfind('.') != std::string::npos)
// 		{
// 			extension = this->filename.substr(this->filename.rfind('.')+1);
// 		}
// 	}
// 
// 	FileRefData::~FileRefData()
// 	{
// 	}
// 
// 	void* FileRefData::getData() const
// 	{
// 		return const_cast<void*>(data);
// 	}
// 
// 	int FileRefData::getSize() const
// 	{
// 		return size;
// 	}
// 
// 	const std::string & FileRefData::getFilename() const
// 	{
// 		return filename;
// 	}
// 
// 	const std::string & FileRefData::getExtension() const
// 	{
// 		return extension;
// 	}
// 
// } // filesystem
// } // love
