#pragma once

#include "Resource.h"

Resource::Resource(const std::string & file)
{
	SetID(file);
}

Resource::~Resource()
{

}

hashID Resource::StringToID(std::string _string)
{
	return std::hash<std::string>{}(_string);
}

hashID Resource::CombineHashes(std::string const & path1, std::string const & path2)
{
	return CombineHashes(StringToID(path1), StringToID(path2));
}

hashID Resource::CombineHashes(hashID hash1, hashID hash2)
{
	return hash1 ^ (hash2 << 1);
}

hashID Resource::GetID() const
{
	return ID;
}

bool Resource::sortFunction(Resource * resA, Resource * resB)
{
	return resA->ID < resB->ID;
}

bool Resource::HasID(hashID id)
{
	return (ID == id);
}

void Resource::SetID(std::string const& path)
{
	ID = StringToID(path);
}

void Resource::SetID(std::string const& path1, std::string const& path2)
{
	ID = StringToID(path1) + StringToID(path2);
}