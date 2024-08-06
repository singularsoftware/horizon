/*******************************************************************************
* Copyright (C) 2024 Singular Software Limited.
*
* Subject to your compliance with these terms, you may use Singular-Software
* software and any derivatives exclusively with Singular-Software products. 
* It is your responsibility to comply with third party license terms applicable 
* to your use of third party software (including open source software) that may 
* accompany Singular-Software software.
*
* THIS SOFTWARE IS SUPPLIED BY SINGULAR-SOFTWARE "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR 
* PURPOSE.
*
* IN NO EVENT WILL SINGULAR-SOFTWARE BE LIABLE FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF SINGULAR-SOFTWARE
* HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, SINGULAR-SOFTWARE'S TOTAL LIABILITY ON ALL 
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES,
* IF ANY, THAT YOU HAVE PAID DIRECTLY TO SINGULAR-SOFTWARE FOR THIS SOFTWARE.
*******************************************************************************/

using Microsoft.Win32.SafeHandles;
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace HorizonDemo
{
    internal class WindowsApi
    {

        public const Int32 FILE_FLAG_OVERLAPPED = 0x40000000;
        public const Int32 FILE_SHARE_READ = 1;
        public const Int32 FILE_SHARE_WRITE = 2;
        public const UInt32 GENERIC_READ = 0x80000000;
        public const UInt32 GENERIC_WRITE = 0x40000000;
        public const Int32 INVALID_HANDLE_VALUE = -1;
        public const Int32 OPEN_EXISTING = 3;
        public const Int32 WAIT_TIMEOUT = 0x102;
        public const Int32 WAIT_OBJECT_0 = 0;
        public const Int32 DIGCF_PRESENT = 2;
        public const Int32 DIGCF_DEVICEINTERFACE = 0X10;
        public const Int32 DBT_DEVICEARRIVAL = 0x8000;
        public const Int32 DBT_DEVICEREMOVECOMPLETE = 0x8004;
        public const Int32 DBT_DEVTYP_DEVICEINTERFACE = 5;
        public const Int32 DBT_DEVTYP_HANDLE = 6;
        public const Int32 DEVICE_NOTIFY_ALL_INTERFACE_CLASSES = 4;
        public const Int32 DEVICE_NOTIFY_SERVICE_HANDLE = 1;
        public const Int32 DEVICE_NOTIFY_WINDOW_HANDLE = 0;
        public const Int32 WM_DEVICECHANGE = 0x219;


        public struct SP_DEVICE_INTERFACE_DATA
        {
            public Int32 cbSize;
            public System.Guid InterfaceClassGuid;
            public Int32 Flags;
            public IntPtr Reserved;
        }


        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_DEVICEINTERFACE_W
        {
            public Int32 dbcc_size;
            public Int32 dbcc_devicetype;
            public Int32 dbcc_reserved;
            public Guid dbcc_classguid;
            public Int16 dbcc_name;
        }


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        public class DEV_BROADCAST_DEVICEINTERFACE_A
        {
            public Int32 dbcc_size;
            public Int32 dbcc_devicetype;
            public Int32 dbcc_reserved;
            [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U1, SizeConst = 16)]
            public Byte[] dbcc_classguid;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 255)]
            public Char[] dbcc_name;
        }

        /// <summary>
        /// The members of the DEV_BROADCAST_HDR structure are contained in each device management structure
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public class DEV_BROADCAST_HDR
        {
            public Int32 dbch_size;
            public Int32 dbch_devicetype;
            public Int32 dbch_reserved;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class SECURITY_ATTRIBUTES
        {
            public Int32 nLength;
            public Int32 lpSecurityDescriptor;
            public Int32 bInheritHandle;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct HIDD_ATTRIBUTES
        {
            public Int32 Size;
            public UInt16 VendorId;
            public UInt16 ProductId;
            public UInt16 VersionNumber;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct HIDP_CAPS
        {
            public Int16 Usage;
            public Int16 UsagePage;
            public Int16 InputReportByteLength;
            public Int16 OutputReportByteLength;
            public Int16 FeatureReportByteLength;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 17)]
            public Int16[] Reserved;
            public Int16 NumberLinkCollectionNodes;
            public Int16 NumberInputButtonCaps;
            public Int16 NumberInputValueCaps;
            public Int16 NumberInputDataIndices;
            public Int16 NumberOutputButtonCaps;
            public Int16 NumberOutputValueCaps;
            public Int16 NumberOutputDataIndices;
            public Int16 NumberFeatureButtonCaps;
            public Int16 NumberFeatureValueCaps;
            public Int16 NumberFeatureDataIndices;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int Left;
            public int Top;
            public int Right;
            public int Bottom;
        }
        public enum ShowMode
        {
            SW_SHOWNORMAL = 1,
            SW_SHOWMINIMIZED = 2,
            SW_SHOWMAXIMIZED = 3
        }

        public delegate bool EnumWinCallBack(IntPtr hwnd, IntPtr lParam);

        [DllImport("user32.Dll")]
        public static extern int EnumChildWindows(IntPtr hWndParent, EnumWinCallBack callBackFunc, IntPtr lParam);

        [DllImport("user32.dll")]
        public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

        [DllImport("user32.dll")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        [DllImport("user32.dll")]
        public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);

        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr RegisterDeviceNotification(IntPtr hRecipient, IntPtr NotificationFilter, Int32 Flags);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern Boolean UnregisterDeviceNotification(IntPtr Handle);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Int32 CancelIo(SafeFileHandle hFile);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern IntPtr CreateEvent(IntPtr SecurityAttributes, Boolean bManualReset, Boolean bInitialState, String lpName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern SafeFileHandle CreateFile(String lpFileName, UInt32 dwDesiredAccess, Int32 dwShareMode, IntPtr lpSecurityAttributes, Int32 dwCreationDisposition, Int32 dwFlagsAndAttributes, Int32 hTemplateFile);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern Boolean GetOverlappedResult(SafeFileHandle hFile, IntPtr lpOverlapped, ref Int32 lpNumberOfBytesTransferred, Boolean bWait);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Boolean ReadFile(SafeFileHandle hFile, IntPtr lpBuffer, Int32 nNumberOfBytesToRead, ref Int32 lpNumberOfBytesRead, IntPtr lpOverlapped);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Int32 WaitForSingleObject(IntPtr hHandle, Int32 dwMilliseconds);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern Boolean WriteFile(SafeFileHandle hFile, Byte[] lpBuffer, Int32 nNumberOfBytesToWrite, ref Int32 lpNumberOfBytesWritten, IntPtr lpOverlapped);

        [DllImport("kernel32", SetLastError = true)]
        public static extern bool CloseHandle(IntPtr h);

        [DllImport("kernel32", SetLastError = true)]
        public static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport("setupapi.dll", SetLastError = true)]
        internal static extern Int32 SetupDiCreateDeviceInfoList(ref System.Guid ClassGuid, Int32 hwndParent);

        [DllImport("setupapi.dll", SetLastError = true)]
        internal static extern Int32 SetupDiDestroyDeviceInfoList(IntPtr DeviceInfoSet);

        [DllImport("setupapi.dll", SetLastError = true)]
        internal static extern Boolean SetupDiEnumDeviceInterfaces(IntPtr DeviceInfoSet, IntPtr DeviceInfoData, ref System.Guid InterfaceClassGuid, Int32 MemberIndex, ref SP_DEVICE_INTERFACE_DATA DeviceInterfaceData);

        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        internal static extern IntPtr SetupDiGetClassDevs(ref System.Guid ClassGuid, IntPtr Enumerator, IntPtr hwndParent, Int32 Flags);

        [DllImport("setupapi.dll", SetLastError = true, CharSet = CharSet.Auto)]
        internal static extern Boolean SetupDiGetDeviceInterfaceDetail(IntPtr DeviceInfoSet, ref SP_DEVICE_INTERFACE_DATA DeviceInterfaceData, IntPtr DeviceInterfaceDetailData, Int32 DeviceInterfaceDetailDataSize, ref Int32 RequiredSize, IntPtr DeviceInfoData);

        [DllImport("hid.dll", SetLastError = true)]
        internal static extern Boolean HidD_FlushQueue(SafeFileHandle HidDeviceObject);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_FreePreparsedData(IntPtr PreparsedData);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetAttributes(SafeFileHandle HidDeviceObject, ref HIDD_ATTRIBUTES Attributes);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetFeature(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetInputReport(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern void HidD_GetHidGuid(ref System.Guid HidGuid);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetNumInputBuffers(SafeFileHandle HidDeviceObject, ref Int32 NumberBuffers);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_GetPreparsedData(SafeFileHandle HidDeviceObject, ref IntPtr PreparsedData);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetFeature(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetNumInputBuffers(SafeFileHandle HidDeviceObject, Int32 NumberBuffers);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Boolean HidD_SetOutputReport(SafeFileHandle HidDeviceObject, Byte[] lpReportBuffer, Int32 ReportBufferLength);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Int32 HidP_GetCaps(IntPtr PreparsedData, ref HIDP_CAPS Capabilities);

        [DllImport("hid.dll", SetLastError = true)]
        public static extern Int32 HidP_GetValueCaps(Int32 ReportType, Byte[] ValueCaps, ref Int32 ValueCapsLength, IntPtr PreparsedData);

        [DllImport("hid.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool HidD_GetProductString(SafeFileHandle HidDeviceObject, StringBuilder Buffer, uint BufferLength);

        [DllImport("hid.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool HidD_GetManufacturerString(SafeFileHandle HidDeviceObject, StringBuilder Buffer, uint BufferLength);

    }
}
