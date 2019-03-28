#pragma once

#include <string>
#include <memory>

typedef size_t hashID;

class Resource
{
public:
	Resource() = default;
	Resource(const std::string &file);
	virtual ~Resource();

	static hashID StringToID(std::string _string);
	static hashID CombineHashes(std::string const& path1, std::string const& path2);
	static hashID CombineHashes(hashID hash1, hashID hash2);

	bool sortFunction(Resource* resA, Resource* resB);
	bool HasID(hashID id);
	hashID GetID() const;

protected:
	void SetID(std::string const& path);
	void SetID(std::string const& path1, std::string const& path2);
	hashID ID = 0;
};