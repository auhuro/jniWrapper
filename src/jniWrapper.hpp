#pragma once
#include <jni.h>
#include <string>
#include <cstdlib>
#include <utility>
namespace jniWrapper {
	std::string getClassName(                 JNIEnv *env, jobject o, jclass c);
	std::string getClassName2(                JNIEnv *env, jobject entity);

	bool   parse_jstring(                     JNIEnv *env, jstring *sIn, std::string &sOut);

	bool   parse_field_jboolean(              JNIEnv *env, jobject o, const char *fieldName, bool   &sOut);
	bool   parse_field_jint(                  JNIEnv *env, jobject o, const char *fieldName, int    &sOut);
	bool   parse_field_jlong(                 JNIEnv *env, jobject o, const char *fieldName, size_t &sOut);
	bool   parse_field_jfloat(                JNIEnv *env, jobject o, const char *fieldName, float  &sOut);
	bool   parse_field_jfloatArray_withMalloc(JNIEnv *env, jobject o, const char *fieldName, float    **sOut, size_t &sOutCount);
	bool   parse_field_jintArray_withMalloc(  JNIEnv *env, jobject o, const char *fieldName, uint32_t **sOut, size_t &sOutCount);
	bool   parse_field_jbyteArray_withMalloc( JNIEnv *env, jobject o, const char *fieldName, uint8_t  **sOut, size_t &sOutCount);

	bool   parseD_field_jboolean(             JNIEnv *env, jobject o, const char *fieldName, const bool   &defaultValue);
	int    parseD_field_jint(                 JNIEnv *env, jobject o, const char *fieldName, const int    &defaultValue);
	size_t parseD_field_jlong(                JNIEnv *env, jobject o, const char *fieldName, const size_t &defaultValue);
	float  parseD_field_jfloat(               JNIEnv *env, jobject o, const char *fieldName, const float  &defaultValue);



	jfloatArray pack_Floats(JNIEnv *env, size_t size, float   *data);
	jbyteArray pack_Bytes(  JNIEnv *env, size_t size, uint8_t *data);
	jintArray pack_Ints(    JNIEnv *env, size_t size, int     *data);

	bool pack_field_to_jfloatArray(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const float   *f,  size_t fSize);
	bool pack_field_to_jbyteArray( JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const uint8_t *fb, size_t fSize);
	bool pack_field_to_jintArray(  JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const int     *fb, size_t fSize);
	bool pack_field_to_jint(       JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const int      fi);
	bool pack_field_to_jstring(    JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const char *s);

	std::pair<jclass, jobject> classGenerate(JNIEnv *env, const std::string &packageName, const std::string &className);


	void toLowerCase(std::string &s);
}

