#include "jniWrapper.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <cstring>

std::string jniWrapper::getClassName(JNIEnv *env, jobject entity, jclass clazz) {
	jmethodID mid = env->GetMethodID(clazz, "getClass", "()Ljava/lang/Class;");
	jobject clsObj = env->CallObjectMethod(entity, mid);
	jclass clazzz = env->GetObjectClass(clsObj);
	mid = env->GetMethodID(clazzz, "getName", "()Ljava/lang/String;");
	jstring strObj = (jstring) env->CallObjectMethod(clsObj, mid);

	const char* str = env->GetStringUTFChars(strObj, NULL);
	std::string res(str);

	env->ReleaseStringUTFChars(strObj, str);

	return res;
}

std::string jniWrapper::getClassName2(JNIEnv *env, jobject entity) {
	jclass entityClass = env->GetObjectClass(entity); // c classOfClassObject = classObject->objectGetClass();
	jmethodID classedGetClassMethodId = env->GetMethodID(entityClass, "getClass", "()Ljava/lang/Class;");
	jobject entityClassObject = env->CallObjectMethod(entityClass, classedGetClassMethodId); // o classObject = entity->getClass()
	jclass entityClassObjectClass = env->GetObjectClass(entityClassObject); // c classOfClassObject = classObject->objectGetClass();


	jmethodID entityClassClassMethodId = env->GetMethodID(entityClassObjectClass, "getName", "()Ljava/lang/String;"); // 
	jstring strObj = (jstring) env->CallObjectMethod(entityClassObjectClass, entityClassClassMethodId); // s strObj = classObject->getName()

	std::string res;
	if (!jniWrapper::parse_jstring(env, &strObj, res)) {
		res.clear();
	}

	return res;
}

bool jniWrapper::parse_field_jboolean(JNIEnv *env, jobject o, const char *fieldName, bool &sOut) {
	const char *fieldTypeSignature = "Z";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		return false;
	}
	jboolean inFieldValue = env->GetBooleanField(o, inField_Id);
	sOut = inFieldValue;
	return true;
}

bool jniWrapper::parse_field_jint(JNIEnv *env, jobject o, const char *fieldName, int &sOut) {
	const char *fieldTypeSignature = "I";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		return false;
	}
	jint inFieldValue = env->GetIntField(o, inField_Id);
	sOut = inFieldValue;
	return true;
}

bool jniWrapper::parse_field_jlong(JNIEnv *env, jobject o, const char *fieldName, size_t &sOut) {
	const char *fieldTypeSignature = "J";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		return false;
	}
	jlong inFieldValue = env->GetLongField(o, inField_Id);
	sOut = inFieldValue;
	return true;
}

bool jniWrapper::parse_field_jfloat(JNIEnv *env, jobject o, const char *fieldName, float &sOut) {
	const char *fieldTypeSignature = "F";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		return false;
	}
	jfloat inFieldValue = env->GetFloatField(o, inField_Id);
	sOut = inFieldValue;
	return true;
}

bool parse_field_jstring(JNIEnv *env, jobject o, const char *fieldName, std::string &sOut) {
	//    const char *fieldTypeSignature = "Ljava/lang/String;";
	//    jfieldID inField_Id = env->GetFieldID(classIn, fieldName, fieldTypeSignature); // Z = boolean
	//    if (!inField_Id) {
	//        return false;
	//    }
	//    jint inFieldValue = env->GetC(objIn, inField_Id);
	////    if (!inFieldValue) {
	////        return false;
	////    }
	//    sOut = inFieldValue;
	return true;
}

bool jniWrapper::parse_field_jfloatArray_withMalloc(JNIEnv *env, jobject o, const char *fieldName, float **sOut, size_t &sOutCount) {
	const char *fieldTypeSignature = "[F";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		std::cerr << "jfa no field found" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}

	jfloatArray jfa = (jfloatArray) env->GetObjectField(o, inField_Id);
	if (jfa == nullptr) {
		std::cerr << "jfa cannot get floatArray" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	//    std::cout << "jfa before" << std::endl;
	jsize count = env->GetArrayLength(jfa);
	if (count == 0) {
		std::cerr << "jfa count is 0" << std::endl;
		//        std::cout << "jfa after 1" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	sOutCount = count;
	jfloat *f = env->GetFloatArrayElements(jfa, NULL);
	if (f == nullptr) {
		std::cerr << "jfa float aray is null" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	//    std::cout << "jfa after 3" << std::endl;
	*sOut = reinterpret_cast<float *> (std::malloc(sizeof (float) * sOutCount));
	std::memcpy(*sOut, f, sOutCount * sizeof (float));
	//    std::cout << "jfa after 4" << std::endl;
	env->ReleaseFloatArrayElements(jfa, f, 0);
	//    std::cout << "jfa after 5" << std::endl;
	//    env->DeleteLocalRef(*vIn);
	return true;
}

bool jniWrapper::parse_field_jintArray_withMalloc(JNIEnv *env, jobject o, const char *fieldName, uint32_t **sOut, size_t &sOutCount) {
	const char *fieldTypeSignature = "[I";
	jclass mvclass = env->GetObjectClass(o);
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	if (!inField_Id) {
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}


	jintArray jfa = (jintArray) env->GetObjectField(o, inField_Id);
	if (jfa == nullptr) {
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	//    std::cout << "jfa before" << std::endl;
	jsize count = env->GetArrayLength(jfa);
	if (count == 0) {
		//        std::cout << "jfa after 1" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	jint *f = env->GetIntArrayElements(jfa, NULL);
	if (f == nullptr) {
		//        std::cout << "jfa after 2" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	//    std::cout << "jfa after 3" << std::endl;
	sOutCount = count;
	*sOut = reinterpret_cast<uint32_t *> (std::malloc(sizeof (uint32_t) * sOutCount));
	memcpy(*sOut, f, sOutCount * sizeof (uint32_t));
	//std::cout << "jfa after 4" << std::endl;
	env->ReleaseIntArrayElements(jfa, f, 0);
	//    std::cout << "jfa after 5" << std::endl;
	//    env->DeleteLocalRef(*vIn);
	return true;
}

bool jniWrapper::parse_field_jbyteArray_withMalloc(JNIEnv *env, jobject o, const char *fieldName, uint8_t **sOut, size_t &sOutCount) {
	const char *fieldTypeSignature = "[B";
	//    std::cout << __FUNCTION__ << ": A" << std::endl;
	jclass mvclass = env->GetObjectClass(o);
	//    std::cout << __FUNCTION__ << ": B" << std::endl;
	jfieldID inField_Id = env->GetFieldID(mvclass, fieldName, fieldTypeSignature); // Z = boolean
	//    std::cout << __FUNCTION__ << ": C" << std::endl;
	if (!inField_Id) {
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}
	//std::cout << __FUNCTION__ << ": D " << inField_Id << std::endl;

	jbyteArray jfa = (jbyteArray) env->GetObjectField(o, inField_Id);
	//    std::cout << __FUNCTION__ << ": E" << jfa << std::endl;
	if (jfa == nullptr) {
		*sOut = nullptr;
		sOutCount = 0;
		return false;
	}

	//    std::cout << "jfa before" << std::endl;
	jsize count = env->GetArrayLength(jfa);
	//    std::cout << __FUNCTION__ << ": F" << std::endl;
	if (count == 0) {
		//        std::cout << "jfa after 1" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		//        std::cout << __FUNCTION__ << ": G" << std::endl;
		return false;
	}
	//    std::cout << __FUNCTION__ << ": H" << std::endl;
	jbyte *f = env->GetByteArrayElements(jfa, NULL);
	//    std::cout << __FUNCTION__ << ": I" << std::endl;
	if (f == nullptr) {
		//        std::cout << "jfa after 2" << std::endl;
		*sOut = nullptr;
		sOutCount = 0;
		//        std::cout << __FUNCTION__ << ": J" << std::endl;
		return false;
	}
	//    std::cout << "jfa after 3 count = " << count << std::endl;
	sOutCount = count;
	//    std::cout << __FUNCTION__ << ": K" << std::endl;
	*sOut = reinterpret_cast<uint8_t *> (std::malloc(sizeof (uint8_t) * sOutCount));
	//    std::cout << __FUNCTION__ << ": L" << std::endl;
	memcpy(*sOut, f, sOutCount * sizeof (uint8_t));
	//    std::cout << __FUNCTION__ << ": M" << std::endl;
	//std::cout << "jfa after 4" << std::endl;
	env->ReleaseByteArrayElements(jfa, f, 0);
	//    std::cout << __FUNCTION__ << ": N" << std::endl;
	//    std::cout << "jfa after 5" << std::endl;
	//    env->DeleteLocalRef(*vIn);
	return true;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

bool jniWrapper::parseD_field_jboolean(JNIEnv *env, jobject o, const char *fieldName, const bool &defaultValue) {
	bool retVal;
	if (!parse_field_jboolean(env, o, fieldName, retVal)) {
		//        if (verboseLevel != 0) {
		//            std::cout << "parseD_field_jboolean: " << fieldName << " set default value" << std::endl;
		//        }
		return defaultValue;
	}
	//    std::cout << "parseD_field_jboolean: " << fieldName << " parsed OK" << std::endl;
	return retVal;
}

int jniWrapper::parseD_field_jint(JNIEnv *env, jobject o, const char *fieldName, const int &defaultValue) {
	int retVal;
	if (!parse_field_jint(env, o, fieldName, retVal)) {
		//        if (verboseLevel != 0) {
		//            std::cout << "parseD_field_jint: " << fieldName << " set default value" << std::endl;
		//        }
		return defaultValue;
	}
	//    std::cout << "parseD_field_jint: " << fieldName << " parsed OK" << std::endl;
	return retVal;
}

size_t jniWrapper::parseD_field_jlong(JNIEnv *env, jobject o, const char *fieldName, const size_t &defaultValue) {
	size_t retVal;
	if (!parse_field_jlong(env, o, fieldName, retVal)) {
		//        std::cout << "parseD_field_jint: " << fieldName << " set default value" << std::endl;
		return defaultValue;
	}
	//    std::cout << "parseD_field_jint: " << fieldName << " parsed OK" << std::endl;
	return retVal;
}

float jniWrapper::parseD_field_jfloat(JNIEnv *env, jobject o, const char *fieldName, const float &defaultValue) {
	float retVal;
	if (!parse_field_jfloat(env, o, fieldName, retVal)) {
		std::cout << "parseD_field_jfloat: " << fieldName << " set default value" << std::endl;
		return defaultValue;
	}
	//    std::cout << "parseD_field_jfloat: " << fieldName << " parsed OK" << std::endl;
	return retVal;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

bool jniWrapper::parse_jstring(JNIEnv *env, jstring *sIn, std::string &sOut) {
	if (!*sIn) {
		return false;
	}
	//    using std::cout;
	//    using std::endl;
	//    cout << "ps begin" << endl;
	const char *nativeString = env->GetStringUTFChars(*sIn, 0);
	sOut.assign(nativeString);
	env->ReleaseStringUTFChars(*sIn, nativeString);
	//    cout << "ps done" << endl;
	return true;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

jfloatArray jniWrapper::pack_Floats(JNIEnv *env, size_t size, float *data) {
	if (data == nullptr) {
		return nullptr;
	}
	//    std::cout << __FUNCTION__ << ": start " << std::endl;
	using std::cout;
	using std::endl;
	jfloatArray result = env->NewFloatArray(size);
	if (!result || env->ExceptionCheck() != JNI_FALSE) {
		std::cerr << __FUNCTION__ << ": error " << std::endl;
		env->ExceptionClear();
		return NULL;
	}
	//    std::cout << __FUNCTION__ << ": before "<<result<<" OK " << data << std::endl;
	env->SetFloatArrayRegion(result, 0, size, data);
	//    std::cout << __FUNCTION__ << ": OK " << std::endl;
	return result;
}

jbyteArray jniWrapper::pack_Bytes(JNIEnv *env, size_t size, uint8_t *data) {
	if (data == nullptr) {
		return nullptr;
	}
	using std::cout;
	using std::endl;
	jbyteArray result = env->NewByteArray(size);
	if (!result || env->ExceptionCheck() != JNI_FALSE) {
		env->ExceptionClear();
		return NULL;
	}
	env->SetByteArrayRegion(result, 0, size, (jbyte *) data);
	return result;
}

jintArray jniWrapper::pack_Ints(JNIEnv *env, size_t size, int *data) {
	if (data == nullptr) {
		return nullptr;
	}
	using std::cout;
	using std::endl;
	jintArray result = env->NewIntArray(size);
	if (!result || env->ExceptionCheck() != JNI_FALSE) {
		env->ExceptionClear();
		return NULL;
	}
	env->SetIntArrayRegion(result, 0, size, (jint *) data);
	return result;
}

//    const char *fieldType = "Ljava/lang/String;";

bool jniWrapper::pack_field_to_jfloatArray(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const float *f, size_t fSize) {
	if (f == nullptr) {
		return false;
	}
	std::cout << __FUNCTION__ << ": start " << fieldName << std::endl;
	const char *fieldType = "[F";
	jfieldID fid = env->GetFieldID(ent_clazz, fieldName, fieldType);
	if (!fid) {
		std::cerr << __FUNCTION__ << ": field not found: " << fieldName << std::endl;
		return false;
	}
	jfloatArray jfa = jniWrapper::pack_Floats(env, fSize, const_cast<float *> (f));
	if (!jfa) {
		std::cerr << __FUNCTION__ << ": cannot create jfa size = " << fSize << std::endl;
	}
	env->SetObjectField(o, fid, jfa);
	std::cout << __FUNCTION__ << ": OK" << std::endl;
	return true;
}

bool jniWrapper::pack_field_to_jbyteArray(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const uint8_t *fb, size_t fSize) {
	if (fb == nullptr) {
		return false;
	}
	std::cout << __FUNCTION__ << ": start " << fieldName << std::endl;
	const char *fieldType = "[B";
	jfieldID fid = env->GetFieldID(ent_clazz, fieldName, fieldType);
	if (!fid) {
		std::cerr << __FUNCTION__ << ": field not found: " << fieldName << std::endl;
		return false;
	}
	jbyteArray jba = jniWrapper::pack_Bytes(env, fSize, const_cast<uint8_t *> (fb));
	if (!jba) {
		std::cerr << __FUNCTION__ << ": cannot create jba size = " << fSize << std::endl;
	}
	env->SetObjectField(o, fid, jba);
	std::cout << __FUNCTION__ << ": OK" << std::endl;
	return true;
}

bool jniWrapper::pack_field_to_jintArray(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const int *fb, size_t fSize) {
	if (fb == nullptr) {
		return false;
	}
	std::cout << __FUNCTION__ << ": start " << fieldName << std::endl;
	const char *fieldType = "[I";
	jfieldID fid = env->GetFieldID(ent_clazz, fieldName, fieldType);
	if (!fid) {
		std::cerr << __FUNCTION__ << ": field not found: " << fieldName << std::endl;
		return false;
	}
	jintArray jba = jniWrapper::pack_Ints(env, fSize, const_cast<int *> (fb));
	if (!jba) {
		std::cerr << __FUNCTION__ << ": cannot create jba size = " << fSize << std::endl;
	}
	env->SetObjectField(o, fid, jba);
	std::cout << __FUNCTION__ << ": OK" << std::endl;
	return true;
}

bool jniWrapper::pack_field_to_jint(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const int fi) {
	const char *fieldType = "I";
	jfieldID fid = env->GetFieldID(ent_clazz, fieldName, fieldType);
	if (!fid) {
		std::cerr << __FUNCTION__ << ": field not found: " << fieldName << std::endl;
		return false;
	}
	env->SetIntField(o, fid, fi);
	std::cout << __FUNCTION__ << ": OK: " << fieldName << std::endl;
	return true;
}

bool jniWrapper::pack_field_to_jstring(JNIEnv *env, jclass ent_clazz, jobject o, const char *fieldName, const char *s) {
	const char *fieldType = "Ljava/lang/String;";
	jfieldID fid = (*env).GetFieldID(ent_clazz, fieldName, fieldType);
	if (!fid) {
		std::cerr << __FUNCTION__ << ": field not found: " << fieldName << std::endl;
		return false;
	}
	if (s == nullptr) {
		std::cerr << __FUNCTION__ << ": s == nullptr: setting string to null: " << fieldName << std::endl;
		(*env).SetObjectField(o, fid, NULL);
		return true;
	}
	jstring js = (*env).NewStringUTF(s);
	if (!js) {
		std::cerr << __FUNCTION__ << ": cannot create js: " << fieldName << std::endl;
	}
	(*env).SetObjectField(o, fid, js);
	std::cout << __FUNCTION__ << ": OK: " << fieldName << std::endl;
	return true;
}

std::pair<jclass, jobject> jniWrapper::classGenerate(JNIEnv *env, const std::string &packageName, const std::string &className) {
	std::cout << __FUNCTION__ << ": start" << std::endl;
	std::string classNameInJava = packageName + "/" + className;

	jclass ent_clazz = env->FindClass(classNameInJava.c_str());

	if (!ent_clazz) {
		std::cerr << __FUNCTION__ << ": error: cannot find class " << classNameInJava.c_str() << std::endl;
		return std::make_pair<jclass, jobject>(nullptr, nullptr);
	}
	std::cout << __FUNCTION__ << ": find class: OK = " << classNameInJava.c_str() << std::endl;

	// Get the IDs of the constructor and the _myEntityType field
	jmethodID ent_init = env->GetMethodID(ent_clazz, "<init>", "()V");
	std::cout << __FUNCTION__ << ": ent_init = " << ent_init << std::endl;
	if (!ent_init) {
		std::cerr << __FUNCTION__ << ": class constructor not found" << std::endl;
		return std::make_pair<jclass, jobject>(nullptr, nullptr);
	}
	std::cout << __FUNCTION__ << ": class constructor found" << std::endl;

	// Create a Java MyEntity instance
	jobject o = env->NewObject(ent_clazz, ent_init);
	if (!o || (env->ExceptionCheck() != JNI_FALSE)) {
		std::cerr << __FUNCTION__ << ": no new object" << std::endl;
		return std::make_pair<jclass, jobject>(nullptr, nullptr);
	}
	std::cout << __FUNCTION__ << ": new object created" << std::endl;
	return std::make_pair(ent_clazz, o);
}

void jniWrapper::toLowerCase(std::string &s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
			return std::tolower(c);
			});
}
