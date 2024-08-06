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
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static HorizonDemo.HorizonHid;


namespace HorizonDemo
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            USBState_label.ForeColor = Color.Red;
            USBState_label.Text = "Not Connected";
            EnableDisableAll(false);
            Close_button.Enabled = true;
            CancelFileTransfer_button.Enabled = false;
            Transfer_progressBar.Visible = false;
            CancelFileTransfer_button.Visible = false;
            Progress_label.Visible = false;

            _horizonHid = new HorizonHid(OnHorizonHidStateEvent, OnHorizonHidFileTransferProgressEvent);
        }

        private void ShowError(int error, bool showSuccess = true)
        {
            if (_horizonHid.GetErrorString(error) == "Success" && !showSuccess)
            {
                return;
            }

            MessageBox.Show(this, $"{_horizonHid.GetErrorString(error)}");
        }

        private void GetVersion_button_Click(object sender, EventArgs e)
        {
            string ver = "";

            _horizonHid.GetFirmwareVersion(ref ver);
            FirmwareVersion_label.Text = ver;
        }

        private void SDCardOpenFile_button_Click(object sender, EventArgs e)
        {
            string fileName = FileName_textBox.Text;
            int fileSize = 0;
            ShowError(_horizonHid.SDCardOpenFile(fileName, ref fileSize));
        }
        private void SDCardCloseFile_button_Click(object sender, EventArgs e)
        {
            ShowError(_horizonHid.SDCardCloseFile());
        }
        private void SDCardWriteFile_button_Click(object sender, EventArgs e)
        {
            string data = SDCardFileData_textBox.Text;

            ShowError(_horizonHid.SDCardWriteFile(data, false));
        }
        private void SDCardReadFile_button_Click(object sender, EventArgs e)
        {
            string data = SDCardFileData_textBox.Text;

            ShowError(_horizonHid.SDCardReadFile(ref data));
            SDCardFileData_textBox.Text = data;
        }
        private void SDCardNewFile_button_Click(object sender, EventArgs e)
        {
            string fileName = FileName_textBox.Text;

            ShowError(_horizonHid.SDCardNewFile(fileName));
        }
        private void SDCardDeleteFile_button_Click(object sender, EventArgs e)
        {
            string fileName = FileName_textBox.Text;

            ShowError(_horizonHid.SDCardDeleteFile(fileName));

        }
        private void SDCardSendFile_button_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog()
            {
                Title = "Send File",
                Filter = "All(*.*)|*"
            };

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                EnableDisableAll(false);

                new Task(() => { _horizonHid.TransferFileToSdCard(dialog.FileName); }).Start();
            }
        }
        private void SDCardGetFile_button_Click(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog()
            {
                Title = "Save File",
                Filter = "All(*.*)|*"
            };

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                EnableDisableAll(false);

                new Task(() => { _horizonHid.TransferFileFromSdCard(dialog.FileName); }).Start();
            }
        }

        private void FlashErase_button_Click(object sender, EventArgs e)
        {
            UInt32 address = UInt32.Parse(FlashEraseAddress_textBox.Text);
            UInt32 blockCount = UInt32.Parse(FlashEraseBlockCount_textBox.Text);

            UInt32 eraseBlockSize = 0;
            EnableDisableAll(false);

            try
            {
                int error = _horizonHid.GetFlashEraseBlockSize(ref eraseBlockSize);
                if (error == 0)
                {
                    if (address % eraseBlockSize != 0)
                    {
                        MessageBox.Show($"Address must be a erase block start addresss. (erase block size = {eraseBlockSize})");
                    }
                    else
                    {
                        ShowError(_horizonHid.FlashErase(address, blockCount));
                    }
                }
            }
            finally
            {
                EnableDisableAll(true);
            }
        }
        private void FlashWrite_button_Click(object sender, EventArgs e)
        {
            EnableDisableAll(false);

            try
            {
                UInt32 address = UInt32.Parse(FlashWriteAddress_textBox.Text);

                byte[] data = Encoding.ASCII.GetBytes(FlashWriteReadData_textBox.Text);

                ShowError(_horizonHid.FlashWrite(address, data));
            }
            finally
            {
                EnableDisableAll(true);
            }
        }
        private void FlashRead_button_Click(object sender, EventArgs e)
        {
            EnableDisableAll(false);

            try
            {
                UInt32 address = UInt32.Parse(FlashReadAddress_textBox.Text);
                UInt32 length = UInt32.Parse(FlashReadLength_textBox.Text);
                byte[] data = new byte[0];

                ShowError(_horizonHid.FlashRead(address, ref data, length));
                FlashWriteReadData_textBox.Text = Encoding.ASCII.GetString(data); ;
            }
            finally
            {
                EnableDisableAll(true);
            }
        }
        private void FlashSendFile_button_Click(object sender, EventArgs e)
        {
            UInt32 address = UInt32.Parse(FlashWriteAddress_textBox.Text);

            OpenFileDialog dialog = new OpenFileDialog()
            {
                Title = "Send File",
                Filter = "All(*.*)|*"
            };

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                FileInfo info = new FileInfo(dialog.FileName);
                FlashGetFileLength_textBox.Text = info.Length.ToString();

                EnableDisableAll(false);

                new Task(() => { _horizonHid.TransferFileToFlash(dialog.FileName, address); }).Start();
            }
        }
        private void FlashGetFile_button_Click(object sender, EventArgs e)
        {
            UInt32 address = UInt32.Parse(FlashReadAddress_textBox.Text);
            UInt32 length = UInt32.Parse(FlashGetFileLength_textBox.Text);

            SaveFileDialog dialog = new SaveFileDialog()
            {
                Title = "Save File",
                Filter = "All(*.*)|*"
            };

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                EnableDisableAll(false);

                new Task(() => { _horizonHid.TransferFileFromFlash(dialog.FileName, address, length); }).Start();
            }
        }


        private void Close_button_Click(object sender, EventArgs e)
        {
            _horizonHid.CloseDevice();
            Close();
        }


        private void CancelFileTransfer_button_Click(object sender, EventArgs e)
        {
            _horizonHid.CancelFileTransfer = true;
        }

        public delegate void HorizonHidStateEventDelegate(object sender, HorizonHidStateArgs e);
        private void OnHorizonHidStateEvent(object sender, HorizonHidStateArgs e)
        {
            if (InvokeRequired)
            {
                Invoke(new HorizonHidStateEventDelegate(this.OnHorizonHidStateEvent), sender, e);
            }
            else
            {
                if (e.DeviceAttached)
                {
                    USBState_label.ForeColor = Color.Green;
                    USBState_label.Text = "Connected";
                    EnableDisableAll(true);
                    CancelFileTransfer_button.Enabled = true;
                }
                else
                {
                    USBState_label.ForeColor = Color.Red;
                    USBState_label.Text = "Not Connected";
                    EnableDisableAll(false);
                    Close_button.Enabled = true;
                    CancelFileTransfer_button.Enabled = false;
                }
            }
        }


        public delegate void HorizonHidFileTransferProgressEventDelegate(object sender, HorizonHidProgressArgs e);
        private void OnHorizonHidFileTransferProgressEvent(object sender, HorizonHidProgressArgs e)
        {
            if (InvokeRequired)
            {
                Invoke(new HorizonHidFileTransferProgressEventDelegate(this.OnHorizonHidFileTransferProgressEvent), sender, e);
            }
            else
            {
                Transfer_progressBar.Visible = true;
                CancelFileTransfer_button.Visible = true;
                Progress_label.Visible = true;

                Transfer_progressBar.Minimum = (int)e.Min;
                Transfer_progressBar.Maximum = (int)e.Max;
                Transfer_progressBar.Value = (int)e.Value;

                Progress_label.Text = e.Message;

                if(e.Done)
                {
                    ShowError(e.Result, true);
                    Transfer_progressBar.Visible = false;
                    CancelFileTransfer_button.Visible = false;
                    Progress_label.Visible = false;
                    Progress_label.Text = "...";
                    EnableDisableAll(true);
                }
            }
        }


        private void Numeric_textBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
            }
        }

        private void EnableDisableAll(bool val)
        {
            if(val)
            {
                Cursor.Current = Cursors.Default;
            }
            else
            {
                Cursor.Current = Cursors.WaitCursor;
            }

            GetVersion_button.Enabled = val;
            FileName_textBox.Enabled = val;
            SDCardOpenFile_button.Enabled = val;
            SDCardDeleteFile_button.Enabled = val;
            SDCardNewFile_button.Enabled = val;
            SDCardReadFile_button.Enabled = val;
            SDCardWriteFile_button.Enabled = val;
            SDCardCloseFile_button.Enabled = val;
            SDCardFileData_textBox.Enabled = val;
            SDCardGetFile_button.Enabled = val;
            SDCardSendFile_button.Enabled = val;

            FlashErase_button.Enabled = val;
            FlashEraseBlockCount_textBox.Enabled = val;
            FlashEraseAddress_textBox.Enabled = val;
            FlashRead_button.Enabled = val;
            FlashWrite_button.Enabled = val;
            FlashReadLength_textBox.Enabled = val;
            FlashWriteAddress_textBox.Enabled = val;
            FlashWriteReadData_textBox.Enabled = val;
            FlashReadAddress_textBox.Enabled = val;
            FlashGetFile_button.Enabled = val;
            FlashSendFile_button.Enabled = val;
            FlashGetFileLength_textBox.Enabled = val;

            Transfer_progressBar.Enabled = val;

            Close_button.Enabled = val;
        }


        private HorizonHid _horizonHid;
    }
}