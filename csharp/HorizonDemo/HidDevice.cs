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
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace HorizonDemo
{
    public class HidDevice : Control
    {
        public HidDevice()
        {
            _deviceData.manufacturer = "Singular Software.";
            _deviceData.product = "Horizon HID Device Demo";

            _deviceData.vid = 0x04D8;
            _deviceData.pid = 0x003F;
            _deviceData.notificationHandle = (IntPtr)IntPtr.Zero.ToInt64();
            _deviceData.attached = false;

            _waitForSingleObjectTimeout = 3000;

            RegisterForNotifications();
        }

        ~HidDevice()
        {
            DetachDevice();
            WindowsApi.UnregisterDeviceNotification(_deviceData.notificationHandle);
        }

        public int ReponseWaitTime { get { return _waitForSingleObjectTimeout; } set { _waitForSingleObjectTimeout = value; } }

        public bool Attached { get { return _deviceData.attached; } }

        public bool FindHidDevices()
        {
            String[] listOfDevicePathNames = new String[128];

            try
            {
                _deviceData.pathName = "";
                List<String> devicesList = new List<string>(); ;

                if (GetHidList(ref devicesList))
                {
                    foreach (string deviceName in devicesList)
                    {
                        _deviceData.hidHandle = WindowsApi.CreateFile(deviceName, 0, WindowsApi.FILE_SHARE_READ | WindowsApi.FILE_SHARE_WRITE, IntPtr.Zero, WindowsApi.OPEN_EXISTING, 0, 0);

                        if (!_deviceData.hidHandle.IsInvalid)
                        {
                            StringBuilder manufacturer = new StringBuilder(256);
                            StringBuilder product = new StringBuilder(256);
                            _deviceData.attributes.Size = Marshal.SizeOf(_deviceData.attributes);

                            if (WindowsApi.HidD_GetAttributes(_deviceData.hidHandle, ref _deviceData.attributes) &&
                               WindowsApi.HidD_GetManufacturerString(_deviceData.hidHandle, manufacturer, 256) &&
                               WindowsApi.HidD_GetProductString(_deviceData.hidHandle, product, 256))
                            {
                                if ((_deviceData.attributes.VendorId == _deviceData.vid) && (_deviceData.attributes.ProductId == _deviceData.pid) &&
                                    _deviceData.manufacturer == manufacturer.ToString() && _deviceData.product == product.ToString())
                                {
                                    _deviceData.pathName = deviceName;
                                    break;
                                }
                            }

                            _deviceData.hidHandle.Close();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                _deviceData.pathName = "";
                Debug.WriteLine($"Find() Exception: {e.Message}");
            }

            return _deviceData.pathName.Length > 0;
        }
        public bool AttachDevice()
        {
            try
            {
                if (_deviceData.pathName.Length > 0)
                {
                    _deviceData.readHandle = WindowsApi.CreateFile(_deviceData.pathName, WindowsApi.GENERIC_READ,
                                             WindowsApi.FILE_SHARE_READ | WindowsApi.FILE_SHARE_WRITE, IntPtr.Zero, WindowsApi.OPEN_EXISTING,
                                             WindowsApi.FILE_FLAG_OVERLAPPED, 0);

                    if (_deviceData.readHandle.IsInvalid)
                    {
                        return false;
                    }
                    else
                    {
                        _deviceData.writeHandle = WindowsApi.CreateFile(_deviceData.pathName, WindowsApi.GENERIC_WRITE,
                                                    WindowsApi.FILE_SHARE_READ | WindowsApi.FILE_SHARE_WRITE, IntPtr.Zero,
                                                    WindowsApi.OPEN_EXISTING, 0, 0);

                        if (_deviceData.writeHandle.IsInvalid)
                        {
                            _deviceData.writeHandle.Close();
                            return false;
                        }
                        else
                        {
                            _deviceData.attached = true;
                            OnHidDeviceEvent(EventArgs.Empty);
                        }
                    }

                }
            }
            catch (Exception e)
            {
                Debug.WriteLine($"AttachDevice() Exception: {e.Message}");
            }
            return true;
        }
        public bool WriteToDevice(Byte[] outputReportBuffer, ref int numberOfBytesWritten)
        {
            if (_deviceData.attached)
            {
                try
                {
                    return WindowsApi.WriteFile(_deviceData.writeHandle, outputReportBuffer,
                                                outputReportBuffer.Length, ref numberOfBytesWritten, IntPtr.Zero);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"WriteToDevice() Exception: {ex.Message}");
                }
            }

            return false;
        }
        public bool ReadFromDevice(ref Byte[] inputReportBuffer, ref int numberOfBytesRead, int waitForSingleObjectTimeout =-1)
        {
            IntPtr eventObject = IntPtr.Zero;
            IntPtr nonManagedBuffer = IntPtr.Zero;
            IntPtr nonManagedOverlapped = IntPtr.Zero;
            NativeOverlapped hidOverlapped = new NativeOverlapped();

            if (_deviceData.attached)
            {
                try
                {
                    numberOfBytesRead = 0;
                    eventObject = WindowsApi.CreateEvent(IntPtr.Zero, false, false, "");
                    hidOverlapped.OffsetLow = 0;
                    hidOverlapped.OffsetHigh = 0;
                    hidOverlapped.EventHandle = eventObject;

                    nonManagedBuffer = Marshal.AllocHGlobal(inputReportBuffer.Length);
                    nonManagedOverlapped = Marshal.AllocHGlobal(Marshal.SizeOf(hidOverlapped));
                    Marshal.StructureToPtr(hidOverlapped, nonManagedOverlapped, false);

                    if (WindowsApi.ReadFile(_deviceData.readHandle, nonManagedBuffer, inputReportBuffer.Length,
                                                  ref numberOfBytesRead, nonManagedOverlapped))
                    {
                        Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);
                    }
                    else
                    {
                        switch (WindowsApi.WaitForSingleObject(eventObject, (waitForSingleObjectTimeout == -1 ? _waitForSingleObjectTimeout : waitForSingleObjectTimeout)))
                        {
                            case (System.Int32)WindowsApi.WAIT_OBJECT_0:
                                WindowsApi.GetOverlappedResult(_deviceData.readHandle, nonManagedOverlapped, ref numberOfBytesRead, false);
                                Marshal.Copy(nonManagedBuffer, inputReportBuffer, 0, numberOfBytesRead);
                                break;

                            case WindowsApi.WAIT_TIMEOUT:
                                WindowsApi.CancelIo(_deviceData.readHandle);
                                //reset
                                DetachDevice(true);
                                AttachDevice();
                                break;

                            default:
                                //reset
                                DetachDevice();
                                AttachDevice();
                                break;
                        }
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"ReadFromDevice() Exception: {ex.Message}");
                }

            }

            Marshal.FreeHGlobal(nonManagedBuffer);
            Marshal.FreeHGlobal(nonManagedOverlapped);
            WindowsApi.CloseHandle(eventObject);

            return numberOfBytesRead > 0;
        }

        public delegate void HidDeviceEventsHandler(object sender, EventArgs e);
        public event HidDeviceEventsHandler HidDeviceEvent;
        private void OnHidDeviceEvent(EventArgs e)
        {
            if (HidDeviceEvent != null)
            {
                HidDeviceEvent(this, e);
            }
        }

        private bool GetHidList(ref List<String> devicesList)
        {
            DetachDevice();
            String[] listOfDevicePathNames = new String[11];
            Int32 bufferSize = 0;
            Int32 listIndex = 0;
            IntPtr detailDataBuffer = IntPtr.Zero;
            IntPtr deviceInfoSet = new IntPtr();
            System.Guid systemHidGuid = new Guid();
            WindowsApi.HidD_GetHidGuid(ref systemHidGuid);
            WindowsApi.SP_DEVICE_INTERFACE_DATA deviceInterfaceData = new WindowsApi.SP_DEVICE_INTERFACE_DATA();

            try
            {
                listIndex = 0;

                deviceInfoSet = WindowsApi.SetupDiGetClassDevs(ref systemHidGuid, IntPtr.Zero, IntPtr.Zero, WindowsApi.DIGCF_PRESENT | WindowsApi.DIGCF_DEVICEINTERFACE);
                deviceInterfaceData.cbSize = Marshal.SizeOf(deviceInterfaceData);
                

                while(WindowsApi.SetupDiEnumDeviceInterfaces(deviceInfoSet, IntPtr.Zero, ref systemHidGuid, listIndex, ref deviceInterfaceData))
                {
                    WindowsApi.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref deviceInterfaceData, IntPtr.Zero, 0, ref bufferSize, IntPtr.Zero);
                    detailDataBuffer = Marshal.AllocHGlobal(bufferSize);
                    Marshal.WriteInt32(detailDataBuffer, (IntPtr.Size == 4) ? (4 + Marshal.SystemDefaultCharSize) : 8);
                    WindowsApi.SetupDiGetDeviceInterfaceDetail(deviceInfoSet, ref deviceInterfaceData, detailDataBuffer, bufferSize, ref bufferSize, IntPtr.Zero);
                    IntPtr pDevicePathName = new IntPtr(detailDataBuffer.ToInt64() + 4);
                    String name = Marshal.PtrToStringAuto(pDevicePathName);
                    if (name != null)
                    {
                        devicesList.Add(name);
                    }
                    listIndex++;
                }
                
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"GetHidList() Exception: {ex.Message}");
                return false;
            }
            finally
            {
                if (detailDataBuffer != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(detailDataBuffer);
                }

                if (deviceInfoSet != IntPtr.Zero)
                {
                    WindowsApi.SetupDiDestroyDeviceInfoList(deviceInfoSet);
                }
            }

            return devicesList.Count > 0;
        }
        public void DetachDevice(bool close = false)
        {
            if (_deviceData.attached || close)
            {
                if (_deviceData.hidHandle != null && !_deviceData.hidHandle.IsInvalid)
                {
                    _deviceData.hidHandle.Close();
                }
                if (_deviceData.readHandle != null && !_deviceData.readHandle.IsInvalid)
                {
                    _deviceData.readHandle.Close();
                }
                if (_deviceData.writeHandle != null && !_deviceData.writeHandle.IsInvalid)
                {
                    _deviceData.writeHandle.Close();
                }

                if (_deviceData.attached)
                {
                    _deviceData.attached = false;
                    OnHidDeviceEvent(EventArgs.Empty);
                }
            }
        }
        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WindowsApi.WM_DEVICECHANGE)
            {
                try
                {
                    switch (m.WParam.ToInt32())
                    {
                        case WindowsApi.DBT_DEVICEARRIVAL:
                            if (!Attached)
                            {
                                if (FindHidDevices())
                                {
                                    AttachDevice();
                                }
                                OnHidDeviceEvent(EventArgs.Empty);
                            }
                            break;

                        case WindowsApi.DBT_DEVICEREMOVECOMPLETE:

                            WindowsApi.DEV_BROADCAST_DEVICEINTERFACE_A devBroadcastDeviceInterface = new WindowsApi.DEV_BROADCAST_DEVICEINTERFACE_A();
                            WindowsApi.DEV_BROADCAST_HDR devBroadcastHeader = new WindowsApi.DEV_BROADCAST_HDR();

                            Marshal.PtrToStructure(m.LParam, devBroadcastHeader);

                            if ((devBroadcastHeader.dbch_devicetype == WindowsApi.DBT_DEVTYP_DEVICEINTERFACE))
                            {
                                Int32 stringSize = System.Convert.ToInt32((devBroadcastHeader.dbch_size - 32) / 2);
                                devBroadcastDeviceInterface.dbcc_name = new Char[stringSize + 1];
                                Marshal.PtrToStructure(m.LParam, devBroadcastDeviceInterface);
                                String deviceNameString = new String(devBroadcastDeviceInterface.dbcc_name, 0, stringSize);
                                String deviceNameStringENUS = deviceNameString.ToLower(new System.Globalization.CultureInfo("en-US"));

                                if (_deviceData.pathName != null)
                                {
                                    if ((String.Compare(deviceNameStringENUS, _deviceData.pathName.ToLower(new System.Globalization.CultureInfo("en-US")), true) == 0))
                                    {
                                        DetachDevice();
                                        OnHidDeviceEvent(EventArgs.Empty);
                                    }
                                }
                            }
                            break;

                        default:
                            break;
                    }
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"WndProc() Exception: {ex.Message}");
                }
            }


            base.WndProc(ref m);
        }
        
        private bool RegisterForNotifications()
        {
            IntPtr devBroadcastDeviceInterfaceBuffer = IntPtr.Zero;
            System.Guid systemHidGuid = new Guid();

            WindowsApi.DEV_BROADCAST_DEVICEINTERFACE_W devBroadcastDeviceInterface = new WindowsApi.DEV_BROADCAST_DEVICEINTERFACE_W();
            WindowsApi.HidD_GetHidGuid(ref systemHidGuid);

            try
            {
                if ((_deviceData.notificationHandle.ToInt64() != IntPtr.Zero.ToInt64()))
                {
                    WindowsApi.UnregisterDeviceNotification(_deviceData.notificationHandle);
                    _deviceData.notificationHandle = (IntPtr)IntPtr.Zero.ToInt64();
                }

                Int32 size = Marshal.SizeOf(devBroadcastDeviceInterface);
                devBroadcastDeviceInterface.dbcc_size = size;
                devBroadcastDeviceInterface.dbcc_devicetype = WindowsApi.DBT_DEVTYP_DEVICEINTERFACE;
                devBroadcastDeviceInterface.dbcc_reserved = 0;
                devBroadcastDeviceInterface.dbcc_classguid = systemHidGuid;

                devBroadcastDeviceInterfaceBuffer = Marshal.AllocHGlobal(size);
                Marshal.StructureToPtr(devBroadcastDeviceInterface, devBroadcastDeviceInterfaceBuffer, true);
                _deviceData.notificationHandle = WindowsApi.RegisterDeviceNotification(this.Handle, devBroadcastDeviceInterfaceBuffer, WindowsApi.DEVICE_NOTIFY_WINDOW_HANDLE);
                Marshal.PtrToStructure(devBroadcastDeviceInterfaceBuffer, devBroadcastDeviceInterface);

                return !(_deviceData.notificationHandle.ToInt64() == IntPtr.Zero.ToInt64());
            }
            catch (Exception ex)
            {
                Debug.WriteLine($"RegisterForNotifications() Exception: {ex.Message}");
            }
            finally
            {
                if (devBroadcastDeviceInterfaceBuffer != IntPtr.Zero)
                {
                    Marshal.FreeHGlobal(devBroadcastDeviceInterfaceBuffer);
                }
            }
            return false;
        }
        private struct DeviceData
        {
            public UInt16 vid;
            public UInt16 pid;
            public string product;
            public string manufacturer;
            public bool attached;
            public String pathName;
            public IntPtr notificationHandle;
            public WindowsApi.HIDD_ATTRIBUTES attributes;
            public WindowsApi.HIDP_CAPS capabilities;
            public SafeFileHandle readHandle;
            public SafeFileHandle writeHandle;
            public SafeFileHandle hidHandle;
        }

        private DeviceData _deviceData;
        private int _waitForSingleObjectTimeout;
    }
}
