project(test_option)

option(ENABLE_FEATURE_A "Enable Feature A" ON)

if (ENABLE_FEATURE_A)
  message(STATUS "Feature A is enabled")
  # 其他相关设置和逻辑
else()
  message(STATUS "Feature A is disabled")
  # 其他相关设置和逻辑
endif()
