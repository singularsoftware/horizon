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

using System;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace HorizonDemo
{
    public class HorizonHid
    {
        public const Int32 MAX_FILENAME_LENGTH = 60;
        private const Int32 MAX_DATA_LENGTH = 50;
        private const Int32 FLASH_RW_DATA_LENGTH = 1024;

        public HorizonHid(HorizonHidStateEventsHandler stateEventsHandler, HorizonHidProgressEventsHandler progressEventHandler)
        {
            HorizonHidStateEvent += stateEventsHandler;
            HorizonHidProgressEvent += progressEventHandler;

            _hidDevice = new HidDevice();
            _hidDevice.HidDeviceEvent += HidDeviceStateEventsHandler;
            if (_hidDevice.FindHidDevices())
            {
                _hidDevice.AttachDevice();
            }
        }
        
        public string GetErrorString(int error)
        {
            if (error >= 0 && error < HorizonErrorString.Length)
            {
                return HorizonErrorString[error];
            }

            return "index out of range";
        }

        public void OpenDevice()
        {
            _hidDevice.AttachDevice();
        }
        public void CloseDevice()
        {
            _hidDevice.DetachDevice(true);
        }


        //FIRMWARE
        public int GetFirmwareVersion(ref string ver)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                packet.Contents[1] = (byte)HorizonUsbMessageId.USB_GET_FIRMWARE_VERSION;
                if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                {
                    if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                    {
                        if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_FIRMWARE_VERSION)
                        {
                            ver = packet.Contents[2].ToString() + "." + packet.Contents[3].ToString() + "." + packet.Contents[4].ToString();
                            return (int)HORIZON_ERROR.ERROR_OK;
                        }
                        else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                        {
                            return packet.Contents[2];
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                        }
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }

        //SD CARD
        public int SDCardOpenFile(string fileName, ref Int32 fileSise)
        {
            if (_hidDevice.Attached)
            {
                if (fileName.Length < MAX_FILENAME_LENGTH && fileName.Length > 0)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_OPEN_FILE;
                    byte[] bytes = Encoding.ASCII.GetBytes(fileName);
                    packet.Contents[2] = (byte)bytes.Length;
                    Array.Copy(bytes, 0, packet.Contents, 3, bytes.Length);
                    packet.Contents[bytes.Length + 3] = 0;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_OPENED)
                            {
                                fileSise = BitConverter.ToInt32(packet.Contents, 2);

                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                return (int)HORIZON_ERROR.USB_ERROR_INVALID_FILENAME;
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardCloseFile()
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_CLOSE_FILE;

                if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                {
                    if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                    {
                        if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_CLOSED)
                        {
                            return (int)HORIZON_ERROR.ERROR_OK;
                        }
                        else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                        {
                            return packet.Contents[2];
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                        }
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardWriteFile(string data, bool flush)
        {
            if (_hidDevice.Attached)
            {
                if (data.Length <= MAX_DATA_LENGTH)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_WRITE_FILE;
                    packet.Contents[2] = ((byte)(flush ? 1 : 0));
                    byte[] bytes = Encoding.ASCII.GetBytes(data);
                    packet.Contents[3] = (byte)bytes.Length;
                    Array.Copy(bytes, 0, packet.Contents, 4, bytes.Length);

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_WRITTEN)
                            {
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                else
                {
                    return (int)HORIZON_ERROR.USB_ERROR_DATA_TOO_LONG;
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardWriteFile(byte[] bytes, bool flush)
        {
            if (_hidDevice.Attached)
            {
                if (bytes.Length <= MAX_DATA_LENGTH)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_WRITE_FILE;
                    packet.Contents[2] = ((byte)(flush ? 1 : 0));
                    packet.Contents[3] = (byte)bytes.Length;
                    Array.Copy(bytes, 0, packet.Contents, 4, bytes.Length);

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_WRITTEN)
                            {
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                else
                {
                    return (int)HORIZON_ERROR.USB_ERROR_DATA_TOO_LONG;
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardReadFile(ref string data)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_READ_FILE;
                packet.Contents[2] = MAX_DATA_LENGTH;

                if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                {
                    if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                    {
                        if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_READ_FILE_READ)
                        {
                            data = Encoding.ASCII.GetString(packet.Contents, 3, packet.Contents[2]);
                            return (int)HORIZON_ERROR.ERROR_OK;
                        }
                        else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                        {
                            return packet.Contents[2];
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                        }
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardReadFile(ref byte[] bytes)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_READ_FILE;
                packet.Contents[2] = MAX_DATA_LENGTH;

                if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                {
                    if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                    {
                        if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_READ_FILE_READ)
                        {
                            bytes = new byte[packet.Contents[2]];
                            Array.Copy(packet.Contents, 3, bytes, 0, packet.Contents[2]);
                            return (int)HORIZON_ERROR.ERROR_OK;
                        }
                        else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                        {
                            return packet.Contents[2];
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                        }
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardNewFile(string fileName)
        {
            if (_hidDevice.Attached)
            {
                if (fileName.Length < MAX_FILENAME_LENGTH && fileName.Length > 0)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_NEW_FILE;
                    byte[] bytes = Encoding.ASCII.GetBytes(fileName);
                    packet.Contents[2] = (byte)bytes.Length;
                    Array.Copy(bytes, 0, packet.Contents, 3, bytes.Length);
                    packet.Contents[bytes.Length + 3] = 0;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_CREATED)
                            {
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                else
                {
                    return (int)HORIZON_ERROR.USB_ERROR_INVALID_FILENAME;
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int SDCardDeleteFile(string fileName)
        {
            if (_hidDevice.Attached)
            {
                if (fileName.Length < MAX_FILENAME_LENGTH && fileName.Length > 0)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_SDCARD_DELETE_FILE;
                    byte[] bytes = Encoding.ASCII.GetBytes(fileName);
                    packet.Contents[2] = (byte)bytes.Length;
                    Array.Copy(bytes, 0, packet.Contents, 3, bytes.Length);
                    packet.Contents[bytes.Length + 3] = 0;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_SDCARD_FILE_DELETED)
                            {
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                else
                {
                    return (int)HORIZON_ERROR.USB_ERROR_INVALID_FILENAME;
                }
            }

            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int TransferFileToSdCard(string fileName)
        {
            CancelFileTransfer = false;
            int result = SDCardNewFile(Path.GetFileName(fileName));

            HorizonHidProgressArgs args = new HorizonHidProgressArgs()
            {
                Min = 0,
                Max = 100,
                Value = 0,
                Done = false,
                Result = 0,
                Message = ""
            };

            if (result == (int)HORIZON_ERROR.ERROR_OK)
            {
                BinaryReader file = null;

                try
                {
                    FileInfo info = new FileInfo(fileName);
                    args.Max = info.Length;

                    file = new BinaryReader(File.Open(fileName, FileMode.Open, FileAccess.Read, FileShare.Read));

                    //first read
                    byte[] bytes = file.ReadBytes(MAX_DATA_LENGTH);

                    while (bytes.Length > 0)
                    {
                        result = SDCardWriteFile(bytes, false);
                        if (result != (int)HORIZON_ERROR.ERROR_OK)
                        {
                            break;
                        }
                        if (CancelFileTransfer)
                        {
                            result = (int)HORIZON_ERROR.USB_ACTION_CANCELLED;
                            break;
                        }

                        //update GUI
                        args.Value += bytes.Length;
                        args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                        OnHorizonHidProgressEvent(args);

                        //next read
                        bytes = file.ReadBytes(MAX_DATA_LENGTH);
                    }

                    //update GUI
                    args.Value = info.Length;
                    args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                    OnHorizonHidProgressEvent(args);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"TransferFile() Exception: {ex.Message}");
                    result = (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                }
                finally
                {
                    file?.Close();
                }

                SDCardCloseFile();

                //update GUI
                args.Done = true;
                args.Result = result;
                args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                OnHorizonHidProgressEvent(args);

                return (int)result;
            }

            //update GUI
            args.Done = true;
            args.Result = result;
            args.Message = "";
            OnHorizonHidProgressEvent(args);

            return result;
        }
        public int TransferFileFromSdCard(string fileName)
        {
            CancelFileTransfer = false;
            int fileSize = 0;
            int result = SDCardOpenFile(Path.GetFileName(fileName), ref fileSize);
            HorizonHidProgressArgs args = new HorizonHidProgressArgs()
            {
                Min = 0,
                Max = fileSize,
                Value = 0,
                Done = false,
                Result = 0,
                Message = ""
            };

            if (result == (int)HORIZON_ERROR.ERROR_OK)
            {
                BinaryWriter file = null;

                try
                {
                    fileName += ".sdcard";
                    file = new BinaryWriter(File.Open(fileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Read));

                    //first read
                    byte[] bytes = new byte[0];

                    result = SDCardReadFile(ref bytes);

                    while (bytes.Length > 0)
                    {
                        file.Write(bytes, 0, bytes.Length);

                        //update GUI
                        args.Value += bytes.Length;
                        args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                        OnHorizonHidProgressEvent(args);

                        //next read
                        result = SDCardReadFile(ref bytes);
                        if (result != (int)HORIZON_ERROR.ERROR_OK)
                        {
                            break;
                        }
                        if (CancelFileTransfer)
                        {
                            result = (int)HORIZON_ERROR.USB_ACTION_CANCELLED;
                            break;
                        }
                    }

                    //update GUI
                    args.Value = fileSize;
                    args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                    OnHorizonHidProgressEvent(args);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"TransferFile(0 Exception: {ex.Message}");
                    result = (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                }
                finally
                {
                    file?.Close();
                }

                SDCardCloseFile();

                //update GUI
                args.Done = true;
                args.Result = result;
                args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                OnHorizonHidProgressEvent(args);

                return (int)result;
            }

            //update GUI
            args.Done = true;
            args.Result = result;
            args.Message = "";
            OnHorizonHidProgressEvent(args);

            return result;
        }

        //
        public bool CancelFileTransfer;
        public class HorizonHidStateArgs : EventArgs
        {
            public bool DeviceAttached { get; set; }
        }
        public delegate void HorizonHidStateEventsHandler(object sender, HorizonHidStateArgs e);
        public event HorizonHidStateEventsHandler HorizonHidStateEvent;
        private void HidDeviceStateEventsHandler(object sender, EventArgs e)
        {
            HorizonHidStateArgs args = new HorizonHidStateArgs()
            {
                DeviceAttached = _hidDevice.Attached
            };

            if (HorizonHidStateEvent != null)
            {
                HorizonHidStateEvent(this, args);
            }
        }


        public class HorizonHidProgressArgs : EventArgs
        {
            public long Min { get; set; }
            public long Max { get; set; }
            public long Value { get; set; }
            public bool Done { get; set; }
            public int Result { get; set; }
            public string Message { get; set; }
        }
        public delegate void HorizonHidProgressEventsHandler(object sender, HorizonHidProgressArgs e);
        public event HorizonHidProgressEventsHandler HorizonHidProgressEvent;
        private void OnHorizonHidProgressEvent(HorizonHidProgressArgs e)
        {
            if (HorizonHidProgressEvent != null)
            {
                HorizonHidProgressEvent(this, e);
            }
        }



        //FLASH
        private uint _flashEraseBlockSize = 0;
        public int GetFlashEraseBlockSize(ref uint blockSize)
        {
            if (_flashEraseBlockSize != 0)
            {
                blockSize = _flashEraseBlockSize;
                return (int)HORIZON_ERROR.ERROR_OK;
            }
            else
            {
                if (_hidDevice.Attached)
                {
                    int numberOfBytes = 0;
                    BasePaket packet = new BasePaket();
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_FLASH_BLOCK_SIZE;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_FLASH_BLOCK_SIZE_IS)
                            {
                                _flashEraseBlockSize = BitConverter.ToUInt32(packet.Contents, 2); ;
                                blockSize = _flashEraseBlockSize;
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                    }
                }
                return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
            }
        }
        public int FlashErase(uint address, uint blockCount)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                packet.Contents[1] = (byte)HorizonUsbMessageId.USB_ERASE_FLASH;

                byte[] addressBytes = BitConverter.GetBytes(address);
                Array.Copy(addressBytes, 0, packet.Contents, 3, sizeof(uint));

                byte[] blockCountBytes = BitConverter.GetBytes(blockCount);
                Array.Copy(blockCountBytes, 0, packet.Contents, 7, sizeof(uint));

                if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                {
                    if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes, (int)(blockCount * 250)))
                    {
                        if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_FLASH_ERASED)
                        {
                            return (int)HORIZON_ERROR.ERROR_OK;
                        }
                        else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                        {
                            return packet.Contents[2];
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                        }
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }

        public int FlashWrite(uint address, byte[] data)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();
                uint dataIndex = 0;
                uint length = (uint)data.Length;

                while (length > 0)
                {
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_WRITE_TO_FLASH;

                    uint dataLength = (uint)Math.Min(length, MAX_DATA_LENGTH);

                    //address
                    byte[] addressBytes = BitConverter.GetBytes(address);
                    Array.Copy(addressBytes, 0, packet.Contents, 3, sizeof(uint));
                    //length
                    byte[] lengthBytes = BitConverter.GetBytes(dataLength);
                    Array.Copy(lengthBytes, 0, packet.Contents, 7, sizeof(uint));
                    //data
                    Array.Copy(data, dataIndex, packet.Contents, 11, dataLength);

                    //update for next write
                    length -= dataLength;
                    address += dataLength;
                    dataIndex += dataLength;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_FLASH_WRITTEN)
                            {
                                if (length > 0)
                                {
                                    continue;
                                }
                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int FlashRead(uint address, ref byte[] data, uint length)
        {
            if (_hidDevice.Attached)
            {
                int numberOfBytes = 0;
                BasePaket packet = new BasePaket();

                data = new byte[length];
                uint dataIndex = 0;

                while (length > 0)
                {
                    uint dataLength = Math.Min(length, MAX_DATA_LENGTH);

                    //read message
                    packet.Contents[1] = (byte)HorizonUsbMessageId.USB_READ_FROM_FLASH;

                    //address
                    byte[] addressBytes = BitConverter.GetBytes(address);
                    Array.Copy(addressBytes, 0, packet.Contents, 3, sizeof(uint));
                    //length
                    byte[] lengthBytes = BitConverter.GetBytes(dataLength);
                    Array.Copy(lengthBytes, 0, packet.Contents, 7, sizeof(uint));

                    //update for next read
                    length -= dataLength;
                    address += dataLength;

                    if (_hidDevice.WriteToDevice(packet.Contents, ref numberOfBytes))
                    {
                        if (_hidDevice.ReadFromDevice(ref packet.Contents, ref numberOfBytes))
                        {
                            if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_FLASH_READ)
                            {
                                Array.Copy(packet.Contents, 11, data, dataIndex, dataLength);
                                dataIndex += dataLength;
                                if (length > 0)
                                {
                                    continue;
                                }

                                return (int)HORIZON_ERROR.ERROR_OK;
                            }
                            else if (packet.Contents[1] == (byte)HorizonUsbMessageId.USB_REQUEST_ERROR)
                            {
                                return packet.Contents[2];
                            }
                            else
                            {
                                return (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                            }
                        }
                        else
                        {
                            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
                        }
                    }
                    else
                    {
                        return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
                    }
                }
                return (int)HORIZON_ERROR.ERROR_OK;
            }
            return (int)HORIZON_ERROR.USB_COMMUNICATION_ERROR;
        }
        public int TransferFileToFlash(string fileName, uint address)
        {
            CancelFileTransfer = false;

            int result = (int)HORIZON_ERROR.ERROR_OK;

            HorizonHidProgressArgs args = new HorizonHidProgressArgs()
            {
                Min = 0,
                Max = 100,
                Value = 0,
                Done = false,
                Result = 0,
                Message = ""
            };

            if (result == (int)HORIZON_ERROR.ERROR_OK)
            {
                BinaryReader file = null;

                try
                {
                    FileInfo info = new FileInfo(fileName);
                    args.Max = info.Length;

                    file = new BinaryReader(File.Open(fileName, FileMode.Open, FileAccess.Read, FileShare.Read));

                    //first read
                    byte[] bytes = file.ReadBytes(FLASH_RW_DATA_LENGTH);

                    while (bytes.Length > 0)
                    {
                        result = FlashWrite(address, bytes);
                        address += (uint)bytes.Length;

                        if (result != (int)HORIZON_ERROR.ERROR_OK)
                        {
                            break;
                        }
                        if (CancelFileTransfer)
                        {
                            result = (int)HORIZON_ERROR.USB_ACTION_CANCELLED;
                            break;
                        }

                        //update GUI
                        args.Value += bytes.Length;
                        args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                        OnHorizonHidProgressEvent(args);

                        //next read
                        bytes = file.ReadBytes(FLASH_RW_DATA_LENGTH);
                    }

                    //update GUI
                    args.Value = info.Length;
                    args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                    OnHorizonHidProgressEvent(args);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"TransferFileToFlash() Exception: {ex.Message}");
                    result = (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                }
                finally
                {
                    file?.Close();
                }

                //update GUI
                args.Done = true;
                args.Result = result;
                args.Message = $"{Path.GetFileName(fileName)} => {args.Max}:{args.Value}";
                OnHorizonHidProgressEvent(args);

                return (int)result;
            }

            //update GUI
            args.Done = true;
            args.Result = result;
            args.Message = "";
            OnHorizonHidProgressEvent(args);

            return result;
        }
        public int TransferFileFromFlash(string fileName, uint address, uint length)
        {
            CancelFileTransfer = false;
            int fileSize = (int)length;
            int result = (int)HORIZON_ERROR.ERROR_OK;
            HorizonHidProgressArgs args = new HorizonHidProgressArgs()
            {
                Min = 0,
                Max = fileSize,
                Value = 0,
                Done = false,
                Result = 0,
                Message = ""
            };

            if (result == (int)HORIZON_ERROR.ERROR_OK)
            {
                BinaryWriter file = null;

                try
                {
                    fileName += ".flash";
                    file = new BinaryWriter(File.Open(fileName, FileMode.Create, FileAccess.ReadWrite, FileShare.Read));

                    uint readLength = length;
                    //first read
                    if (readLength > FLASH_RW_DATA_LENGTH)
                    {
                        readLength = FLASH_RW_DATA_LENGTH;
                    }

                    byte[] bytes = new byte[readLength];

                    result =  FlashRead(address, ref bytes, readLength);

                    while (bytes.Length > 0)
                    {
                        length -= (uint)bytes.Length;
                        address += (uint)bytes.Length;

                        file.Write(bytes, 0, bytes.Length);

                        //update GUI
                        args.Value += bytes.Length;
                        args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                        OnHorizonHidProgressEvent(args);

                        //next read
                        readLength = length;
                        if (readLength > FLASH_RW_DATA_LENGTH)
                        {
                            readLength = FLASH_RW_DATA_LENGTH;
                        }

                        bytes = new byte[readLength];

                        result = FlashRead(address, ref bytes, readLength);

                        if (result != (int)HORIZON_ERROR.ERROR_OK)
                        {
                            break;
                        }
                        if (CancelFileTransfer)
                        {
                            result = (int)HORIZON_ERROR.USB_ACTION_CANCELLED;
                            break;
                        }
                    }

                    //update GUI
                    args.Value = fileSize;
                    args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                    OnHorizonHidProgressEvent(args);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine($"TransferFile(0 Exception: {ex.Message}");
                    result = (int)HORIZON_ERROR.USB_ERROR_UNKNOWN;
                }
                finally
                {
                    file?.Close();
                }

                //update GUI
                args.Done = true;
                args.Result = result;
                args.Message = $"{Path.GetFileName(fileName)} <= {args.Max}:{args.Value}";
                OnHorizonHidProgressEvent(args);

                return (int)result;
            }

            //update GUI
            args.Done = true;
            args.Result = result;
            args.Message = "";
            OnHorizonHidProgressEvent(args);

            return result;
        }
        private enum HorizonUsbMessageId
        {
            USB_GET_FIRMWARE_VERSION = 10,
            USB_FIRMWARE_VERSION,

            USB_SDCARD_OPEN_FILE,
            USB_SDCARD_CLOSE_FILE,
            USB_SDCARD_WRITE_FILE,
            USB_SDCARD_READ_FILE,
            USB_SDCARD_NEW_FILE,
            USB_SDCARD_DELETE_FILE,

            USB_SDCARD_FILE_OPENED,
            USB_SDCARD_FILE_CLOSED,
            USB_SDCARD_FILE_WRITTEN,
            USB_SDCARD_READ_FILE_READ,
            USB_SDCARD_FILE_CREATED,
            USB_SDCARD_FILE_DELETED,

            USB_FLASH_BLOCK_SIZE,
            USB_ERASE_FLASH,
            USB_WRITE_TO_FLASH,
            USB_READ_FROM_FLASH,

            USB_FLASH_BLOCK_SIZE_IS,
            USB_FLASH_ERASED,
            USB_FLASH_WRITTEN,
            USB_FLASH_READ,

            USB_REQUEST_ERROR = 100
        }

        public enum HORIZON_ERROR
        {
            /* Success */
            ERROR_OK = 0,

            /*** SD_CARD FILE SYSTEM ***/
            /* (1) A hard error occurred in the low level disk I/O layer */
            SYS_FS_ERROR_DISK_ERR,
            /* (2) Assertion failed */
            SYS_FS_ERROR_INT_ERR,
            /* (3) The physical drive cannot work */
            SYS_FS_ERROR_NOT_READY,
            /* (4) Could not find the file */
            SYS_FS_ERROR_NO_FILE,
            /* (5) Could not find the path */
            SYS_FS_ERROR_NO_PATH,
            /* (6) The path name format is invalid */
            SYS_FS_ERROR_INVALID_NAME,
            /* (7) Access denied due to prohibited access or directory full */
            SYS_FS_ERROR_DENIED,
            /* (8) Access denied due to prohibited access */
            SYS_FS_ERROR_EXIST,
            /* (9) The file/directory object is invalid */
            SYS_FS_ERROR_INVALID_OBJECT,
            /* (10) The physical drive is write protected */
            SYS_FS_ERROR_WRITE_PROTECTED,
            /* (11) The logical drive number is invalid */
            SYS_FS_ERROR_INVALID_DRIVE,
            /* (12) The volume has no work area */
            SYS_FS_ERROR_NOT_ENABLED,
            /* (13) There is no valid volume */
            SYS_FS_ERROR_NO_FILESYSTEM,
            /* (14) The Format() aborted due to any parameter error */
            SYS_FS_ERROR_FORMAT_ABORTED,
            /* (15) Could not get a grant to access the volume within defined period */
            SYS_FS_ERROR_TIMEOUT,
            /* (16) The operation is rejected according to the file sharing policy */
            SYS_FS_ERROR_LOCKED,
            /* (17) LFN working buffer could not be allocated */
            SYS_FS_ERROR_NOT_ENOUGH_CORE,
            /* (18) Number of open files */
            SYS_FS_ERROR_TOO_MANY_OPEN_FILES,
            /* (19) Given parameter is invalid */
            SYS_FS_ERROR_INVALID_PARAMETER,
            /* (20) Too many mounts requested. Not enough free volume available */
            SYS_FS_ERROR_NOT_ENOUGH_FREE_VOLUME,
            /* (21) Requested native file system is not supported */
            SYS_FS_ERROR_FS_NOT_SUPPORTED,
            /* (22) Requested native file system does not match the format of volume */
            SYS_FS_ERROR_FS_NOT_MATCH_WITH_VOLUME,
            /* (23) Function not supported in native file system layer */
            SYS_FS_ERROR_NOT_SUPPORTED_IN_NATIVE_FS,

            /*** SST26VF032B FLASH ***/
            /* Transfer is being processed */
            DRV_SST26_TRANSFER_BUSY = 50,
            /* Transfer is successfully completed*/
            DRV_SST26_TRANSFER_COMPLETED,
            /* Transfer had error*/
            DRV_SST26_TRANSFER_ERROR_UNKNOWN,

            /*** USB ***/
             /* USB Communication error */
            USB_COMMUNICATION_ERROR = 100,
            /* Unknown error */
            USB_ERROR_UNKNOWN,

            USB_ERROR_INVALID_FILENAME,
            USB_ERROR_DATA_TOO_LONG,
            USB_ACTION_CANCELLED
        }
    
        private string[] HorizonErrorString =
        {
            "Success",
            "A hard error occurred in the low level disk I/O layer",
            "Assertion failed",
            "The physical drive cannot work",
            "Could not find the file",
            "Could not find the path",
            "The path name format is invalid",
            "Access denied due to prohibited access or directory full",
            "Access denied due to prohibited access",
            "The file/directory object is invalid",
            "The physical drive is write protected",
            "The logical drive number is invalid",
            "The volume has no work area",
            "There is no valid volume",
            "The Format() aborted due to any parameter error",
            "Could not get a grant to access the volume within defined period",
            "The operation is rejected according to the file sharing policy",
            "LFN working buffer could not be allocated",
            "Number of open files",
            "Given parameter is invalid",
            "Too many mounts requested. Not enough free volume available",
            "Requested native file system is not supported",
            "Requested native file system does not match the format of volume",
            "Function not supported in native file system layer",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "Transfer is being processed",
            "Transfer is successfully completed",
            "Transfer had errorr",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "USB communication error",
            "Unknown error",
            "Invalid file name",
            "Data too long",
            "Action cancelled"
        };

        private HidDevice _hidDevice;
        public class BasePaket
        {
            public const int USB_PACKET_SIZE = 0x40;
            public BasePaket()
            {
                Contents = new byte[USB_PACKET_SIZE + 1];
            }
            public byte[] Contents;
            /*
             * Contents[0] -> NA
             * Contents[1] -> Message ID
             * Contents[2] -> Data length
             * Contents[3-63] -> Data
             */
        }
    }
}
