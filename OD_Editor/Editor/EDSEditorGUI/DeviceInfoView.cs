/*
    This file is part of libEDSsharp.

    libEDSsharp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libEDSsharp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libEDSsharp.  If not, see <http://www.gnu.org/licenses/>.

    Copyright(c) 2016 - 2019 Robin Cornelius <robin.cornelius@gmail.com>
*/

using System;
using System.Windows.Forms;
using libEDSsharp;
using System.IO;
using System.Runtime.CompilerServices;
using System.Drawing;

namespace ODEditor
{
    public partial class DeviceInfoView : MyTabUserControl
    {
        public EDSsharp eds = null;

        public DeviceInfoView()
        {
            InitializeComponent();
        }

        public void populatedeviceinfo()
        {
            if (eds == null)
                return;

            textBox_productname.Text = eds.di.ProductName;
            textBox_productnumber.Text = eds.di.ProductNumber;
            textBox_vendorname.Text = eds.di.VendorName;
            textBox_vendornumber.Text = eds.di.VendorNumber;

            textBox_fileversion.Text = eds.fi.FileVersion;
            textBox_di_description.Text = eds.fi.Description;
            textBox_create_datetime.Text = eds.fi.CreationDateTime.ToString();
            textBox_createdby.Text = eds.fi.CreatedBy;
            textBox_modified_datetime.Text = eds.fi.ModificationDateTime.ToString();
            textBox_modifiedby.Text = eds.fi.ModifiedBy;

            checkBox_baud_10.Checked = eds.di.BaudRate_10;
            checkBox_baud_20.Checked = eds.di.BaudRate_20;
            checkBox_baud_50.Checked = eds.di.BaudRate_50;
            checkBox_baud_125.Checked = eds.di.BaudRate_125;
            checkBox_baud_250.Checked = eds.di.BaudRate_250;
            checkBox_baud_500.Checked = eds.di.BaudRate_500;
            checkBox_baud_800.Checked = eds.di.BaudRate_800;
            checkBox_baud_1000.Checked = eds.di.BaudRate_1000;
            checkBox_baud_auto.Checked = eds.di.BaudRate_auto;

            textBox_granularity.Text = eds.di.Granularity.ToString();
            textBox_rxpdos.Text = eds.di.NrOfRXPDO.ToString();
            textBox_txpdos.Text = eds.di.NrOfTXPDO.ToString();
            checkBox_lss.Checked = eds.di.LSS_Supported;
            checkBox_lssMaster.Checked = eds.di.LSS_Master;

            textBox_projectFileName.Text = Path.GetFileName(eds.projectFilename);
            if (eds.xddfilename_1_1 != "")
                textBox_projectFileVersion.Text = "v1.1";
            else if (eds.xddfilename_1_0 != "" && eds.xddfilename_1_0 == eds.projectFilename)
                textBox_projectFileVersion.Text = "v1.0";
            else
                textBox_projectFileVersion.Text = "";
            textBox_deviceedsname.Text = Path.GetFileName(eds.edsfilename);
            textBox_xddfilenameStripped.Text = Path.GetFileName(eds.xddfilenameStripped);
            textBox_devicedcfname.Text = Path.GetFileName(eds.dcffilename);
            textBox_canopennodeFileName.Text = Path.GetFileNameWithoutExtension(eds.ODfilename);
            textBox_canopennodeFileVersion.Text = eds.ODfileVersion;
            textBox_mdFileName.Text = Path.GetFileName(eds.mdfilename);

            //DCF support
            if (eds.dc!=null)
            {
                textBox_concretenodeid.Text = eds.dc.NodeID.ToString();
                textBox_nodename.Text = eds.dc.NodeName;
                textBox_baudrate.Text = eds.dc.Baudrate.ToString();
                textBox_netnum.Text = eds.dc.NetNumber.ToString();
                checkBox_canopenmanager.Checked = eds.dc.CANopenManager;
                textBox_lssserial.Text = eds.dc.LSS_SerialNumber.ToString();
            }

            AddOnChangeHandlerToInputControls(this.Controls);

        }


        void InputControls_OnChange(object sender, EventArgs e)
        {
            // Do something to indicate the form is dirty like:
            button_update_devfile_info.BackColor = System.Drawing.Color.Red;
        }

        void AddOnChangeHandlerToInputControls(ControlCollection ctrl)
        {
            foreach (Control subctrl in ctrl)
            {
                //if (subctrl is TextBox)
                //    ((TextBox)subctrl).TextChanged +=
                //        new EventHandler(InputControls_OnChange);
                //else 
                if (subctrl is CheckBox)
                    ((CheckBox)subctrl).CheckedChanged +=
                        new EventHandler(InputControls_OnChange);
                else
                {
                    if (subctrl.Controls.Count > 0)
                        this.AddOnChangeHandlerToInputControls(subctrl.Controls);
                }
            }
    }



        private void button_update_devfile_info_Click(object sender, EventArgs e)
        {
            if (eds == null)
                return;

            try
            {
                eds.di.ProductName = textBox_productname.Text;
                eds.di.ProductNumber = textBox_productnumber.Text;
                eds.di.VendorName = textBox_vendorname.Text;
                eds.di.VendorNumber = textBox_vendornumber.Text;

                eds.fi.FileVersion = textBox_fileversion.Text;
                eds.fi.Description = textBox_di_description.Text;
                eds.fi.CreationDateTime = DateTime.Parse(textBox_create_datetime.Text);
                eds.fi.CreatedBy = textBox_createdby.Text;
                eds.fi.ModifiedBy = textBox_modifiedby.Text;

                eds.di.BaudRate_10 = checkBox_baud_10.Checked;
                eds.di.BaudRate_20 = checkBox_baud_20.Checked;
                eds.di.BaudRate_50 = checkBox_baud_50.Checked;
                eds.di.BaudRate_125 = checkBox_baud_125.Checked;
                eds.di.BaudRate_250 = checkBox_baud_250.Checked;
                eds.di.BaudRate_500 = checkBox_baud_500.Checked;
                eds.di.BaudRate_800 = checkBox_baud_800.Checked;
                eds.di.BaudRate_1000 = checkBox_baud_1000.Checked;
                eds.di.BaudRate_auto = checkBox_baud_auto.Checked;

                eds.di.Granularity = Convert.ToByte(textBox_granularity.Text);
                eds.di.LSS_Supported = checkBox_lss.Checked;
                eds.di.LSS_Master = checkBox_lssMaster.Checked;

                doUpdatePDOs();
               
                //These are read only and auto calculated 
                //textBox_rxpdos.Text = eds.di.NrOfRXPDO.ToString();
                //textBox_txpdos.Text = eds.di.NrOfTXPDO.ToString();

                //DCF support
                eds.dc.NodeID = Convert.ToByte(textBox_concretenodeid.Text);
                eds.dc.NodeName = textBox_nodename.Text;
                eds.dc.Baudrate = Convert.ToUInt16(textBox_baudrate.Text);
                eds.dc.NetNumber = Convert.ToUInt32(textBox_netnum.Text);
                eds.dc.CANopenManager = checkBox_canopenmanager.Checked;
                eds.dc.LSS_SerialNumber = Convert.ToUInt32(textBox_lssserial.Text);

                eds.Dirty = true;
                button_update_devfile_info.BackColor = SystemColors.ButtonFace;
                button_update_devfile_info.UseVisualStyleBackColor = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show("Update failed, reason :-\n" + ex.ToString());
            }
        }
    }
}
