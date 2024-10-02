# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/1"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/tmp"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/src/BC260Y_COD_MQTT+BC260YCODMQTT-stamp"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/src"
  "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/src/BC260Y_COD_MQTT+BC260YCODMQTT-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/src/BC260Y_COD_MQTT+BC260YCODMQTT-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Michael_Chemic/Desktop/BC260Y_COD_MQTT/Remote_data_acquisition_system/MDK-ARM/tmp/BC260Y_COD_MQTT+BC260YCODMQTT/src/BC260Y_COD_MQTT+BC260YCODMQTT-stamp${cfgdir}") # cfgdir has leading slash
endif()
