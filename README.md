# jniWrapper
```c++
#include "jniWrapper/src/jniWrapper.hpp"
```

```c++
#include <cstdlib>
#include <string>

struct ParsedObject {
	bool boolField            = false;
	size_t jlongField         = 0;
	int errorCode             = 0;
	std::string errorDescription;
	float *floatElementsField = nullptr;
	size_t floatElementsFieldCount = 0;

	~ParsedObject() {
		std::free(floatElementsField);
	}
};

void parse(JNIEnv *env, jobject o, ParsedObject &parsedObject) {
	float *f = nullptr;
    uint32_t *i = nullptr;
    uint8_t *b = nullptr;
    size_t aCount = 0;
	// bool
	parsedObject.boolField  = jniWrapper::parseD_field_jboolean(env, o, "boolField"      , false);

	// size_t
	parsedObject.jlongField = jniWrapper::parseD_field_jlong(   env, o, "jlongField"     , 22);

	// float[]
	jniWrapper::parse_field_jfloatArray_withMalloc(             env, o, "floatElementsField", &f, aCount);
	parsedObject.floatElementsField = f;
	parsedObject.floatElementsFieldCount = aCount;
}
```

```c++
#include <string>
static std::string javaPackageForClasses;

// Java class output.
jobject generate(JNIEnv *env, const ParsedObject &parsedObject) {
	std::pair<jclass, jobject> res = jniWrapper::classGenerate(env, javaPackageForClasses, "JavaClassName");
    if (res.second == nullptr) {
        return nullptr;
    }
    jclass ent_clazz = res.first;
    jobject o = res.second;

    jniWrapper::pack_field_to_jfloatArray(env, ent_clazz, o, "floatElementsField",          &parsedObject.floatElementsField[0], parsedObject.floatElementsFieldCount);
	jniWrapper::pack_field_to_jint(env, ent_clazz, o, "errorCode", parsedObject.errorCode);
    jniWrapper::pack_field_to_jstring(env, ent_clazz, o, "errorDescription", parsedObject.errorDescription.c_str());
	return o;
}

JNIEXPORT jobject JNICALL Java_call(JNIEnv *env, jclass objCaller, jobject objIn, jstring callerPackageName)
{
	jniWrapper::parse_jstring(env, &callerPackageName, callerPackageNameS);
	javaPackageForClasses = callerPackageNameS;
	jniWrapper::toLowerCase(javaPackageForClasses);

	ParsedObject parsedObject;

	// parse
	parse(env, objIn, parsedObject);

	// generate
	jobject generatedObject = generate(env, parsedObject);

	return generatedObject;
}
```

