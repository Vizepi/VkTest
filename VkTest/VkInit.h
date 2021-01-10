#pragma once

#define VK_USE_PLATFORM_WIN32_KHR 1
#include <vulkan/vulkan.hpp>
#include <string>

BOOL InitVulkan( HINSTANCE hInst, HWND hWnd ) {
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "VkTest";
    applicationInfo.applicationVersion = VK_MAKE_VERSION( 0, 0, 1 );
    applicationInfo.pEngineName = "VkTestEngine";
    applicationInfo.engineVersion = VK_MAKE_VERSION( 0, 0, 1 );
    applicationInfo.apiVersion = VK_MAKE_VERSION( 1, 2, 0 );

    char const * extensions[]{
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = 0;
    instanceCreateInfo.ppEnabledLayerNames = nullptr;
    instanceCreateInfo.enabledExtensionCount = 2;
    instanceCreateInfo.ppEnabledExtensionNames = extensions;

    VkInstance instance;
    VkResult result;
    if ( ( result = vkCreateInstance( &instanceCreateInfo, nullptr, &instance ) ) != VK_SUCCESS ) {
        OutputDebugStringA( std::to_string( ( int )result ).c_str() );
        OutputDebugStringA( ": Create instance failed.\n" );
        return FALSE;
    }

    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.pNext = nullptr;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.hinstance = hInst;
    surfaceCreateInfo.hwnd = hWnd;

    VkSurfaceKHR surface;
    if ( ( result = vkCreateWin32SurfaceKHR( instance, &surfaceCreateInfo, nullptr, &surface ) ) != VK_SUCCESS ) {
        OutputDebugStringA( std::to_string( ( int )result ).c_str() );
        OutputDebugStringA( ": Create surface failed.\n" );
        vkDestroyInstance( instance, nullptr );
        return FALSE;
    }

    uint32_t physicalDeviceCount{ 1 };
    VkPhysicalDevice physicalDevice;
    if ( ( result = vkEnumeratePhysicalDevices( instance, &physicalDeviceCount, &physicalDevice ) ) != VK_SUCCESS ) {
        OutputDebugStringA( std::to_string( ( int )result ).c_str() );
        OutputDebugStringA( ": Enumerate physical devices failed.\n" );
        vkDestroySurfaceKHR( instance, surface, nullptr );
        vkDestroyInstance( instance, nullptr );
        return false;
    }

    VkPhysicalDeviceSurfaceInfo2KHR surfaceInfo{};
    surfaceInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
    surfaceInfo.pNext = nullptr;
    surfaceInfo.surface = surface;

    uint32_t formatCount{};
    if ( ( result = vkGetPhysicalDeviceSurfaceFormats2KHR( physicalDevice, &surfaceInfo, &formatCount, nullptr ) ) != VK_SUCCESS ) {
        OutputDebugStringA( std::to_string( ( int )result ).c_str() );
        OutputDebugStringA( ": Get formats failed.\n" );
    }
    vkDestroySurfaceKHR( instance, surface, nullptr );
    vkDestroyInstance( instance, nullptr );

    return TRUE;
}
