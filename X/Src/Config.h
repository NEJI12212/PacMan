//====================================================================================================
// Filename:	Config.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef	INCLUDED_XENGINE_CONFIG_H
#define INCLUDED_XENGINE_CONFIG_H

namespace X {

class Config
{
public:
	static void StaticInitialize(const char* fileName);
	static void StaticTerminate();
	static Config* Get();

public:
	Config();

	void Load(const char* fileName);
	void Save();
	void SaveAs(const char* fileName);

	int GetInt(const char* key, int default = 0) const;
	bool GetBool(const char* key, bool default = false) const;
	float GetFloat(const char* key, float default = 0.0f) const;
	const char* GetString(const char* key, const char* default = "") const;
	
	void SetInt(const char* key, int value);
	void SetBool(const char* key, bool value);
	void SetFloat(const char* key, float value);
	void SetString(const char* key, const char* value);

private:
	class Impl;
	std::unique_ptr<Impl> mImpl;
};

} // namespace X

#endif // #ifndef INCLUDED_XENGINE_CONFIG_H