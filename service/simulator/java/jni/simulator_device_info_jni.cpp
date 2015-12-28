/******************************************************************
 *
 * Copyright 2015 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "simulator_device_info_jni.h"
#include "simulator_utils_jni.h"

extern SimulatorClassRefs gSimulatorClassRefs;

jobject JniDeviceInfo::toJava(DeviceInfo &deviceInfo)
{
    if (!m_env)
        return nullptr;

    static jmethodID deviceInfoCtor = m_env->GetMethodID(gSimulatorClassRefs.deviceInfoCls, "<init>",
                                      "()V");
    jobject jDeviceInfo = m_env->NewObject(gSimulatorClassRefs.deviceInfoCls, deviceInfoCtor);
    setFieldValue(jDeviceInfo, "mName", deviceInfo.getName());
    setFieldValue(jDeviceInfo, "mID", deviceInfo.getID());
    setFieldValue(jDeviceInfo, "mSpecVersion", deviceInfo.getSpecVersion());
    setFieldValue(jDeviceInfo, "mDMVVersion", deviceInfo.getDataModelVersion());

    return jDeviceInfo;
}

void JniDeviceInfo::setFieldValue(jobject jDeviceInfo, const std::string &fieldName,
                                  const std::string &value)
{
    jfieldID fieldID = m_env->GetFieldID(m_env->GetObjectClass(jDeviceInfo), fieldName.c_str(),
                                         "Ljava/lang/String;");
    jstring valueStr = m_env->NewStringUTF(value.c_str());
    m_env->SetObjectField(jDeviceInfo, fieldID, valueStr);
}

void onDeviceInfoReceived(jobject listener, const std::string &hostUri, DeviceInfo &deviceInfo)
{
    JNIEnv *env = getEnv();
    if (!env)
        return;

    jclass listenerCls = env->GetObjectClass(listener);
    jmethodID listenerMethodId = env->GetMethodID(listenerCls, "onDeviceFound",
                                 "(Ljava/lang/String;Lorg/oic/simulator/DeviceInfo;)V");


    jstring jHostUri = env->NewStringUTF(hostUri.c_str());
    jobject jDeviceInfo = JniDeviceInfo(env).toJava(deviceInfo);
    if (!jDeviceInfo)
    {
        releaseEnv();
        return;
    }

    env->CallVoidMethod(listener, listenerMethodId, jHostUri, jDeviceInfo);
    if (env->ExceptionCheck())
    {
        releaseEnv();
        return;
    }

    releaseEnv();
}

