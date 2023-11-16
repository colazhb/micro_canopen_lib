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

    Copyright(c) 2016 - 2020 Robin Cornelius <robin.cornelius@gmail.com>
    Copyright(c) 2020 Janez Paternoster
*/

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;
using System.Linq;
using System.Xml;
using System.Net.Http;
using System.Threading.Tasks;
using System.Net;
using System.Web;
using System.Security.Cryptography;
using static libEDSsharp.EDSsharp;
using System.Runtime.Remoting.Messaging;

namespace libEDSsharp
{

        /// <summary>
        /// Exporter for CanOpenNode_V5
        /// </summary>
        public class CanOpenNodeExporter_V5 : IExporter
    {
        private string odname;

        private List<string> od_id_list = new List<string>();
        private List<string> od_id_struct_list = new List<string>();
        private List<ODentry> odentry_list = new List<ODentry>();

        // tpdo str
        private List<string> tpdo_str_list = new List<string>();
        private List<string> tpdo_str_enum = new List<string>();
        private List<string> tpdo_str_cfg = new List<string>();
        
        // rpdo str
        private List<string> rpdo_str_list = new List<string>();
        private List<string> rpdo_str_enum = new List<string>();
        private List<string> rpdo_str_cfg = new List<string>();
        

        private List<string> ODStorageGroups;
        private Dictionary<string, List<string>> ODStorage_t;
        private Dictionary<string, List<string>> ODStorage;

        private List<string> ODObjs_t;
        private List<string> ODObjs;
        private List<string> ODList;
        private List<string> ODDefines;
        private List<string> ODDefinesLong;
        private Dictionary<string, UInt16> ODCnt;
        private Dictionary<string, int> ODArrSize;

 
        /// <summary>
        /// export the current data set in the CanOpen Node format V4
        /// </summary>
        /// <param name="folderpath"></param>
        /// <param name="filename"></param>
        /// <param name="gitVersion"></param>
        /// <param name="eds"></param>
        /// <param name="odname"></param>
        public void export(string folderpath, string filename, string gitVersion, EDSsharp eds, string odname)
        {
            this.odname = odname;

            Prepare(eds);

            Export_h(folderpath, filename, gitVersion, eds);
            Export_c(folderpath, filename, gitVersion, eds);
        }

        private string od_id_to_struct_str(ODentry od)
        {
            //    {0x1001,     0,        OD_DATA_TYPE_UINT8,      OD_RW_ACCESS_RO,  0,      0,        0,        0,       0,        0,      "Error Register", ""},

            string od_id_str = "";
            

            string ch_str = od.parameter_name;

            string ch_trans = od.parameter_name_trans;

            //string od_id = ch_str;

           // od_id = od_id.ToUpper();
           // od_id = od_id.Replace(" ", "_");
           // od_id += ",";
            string indexH = $"{od.Index:X4}";

            string od_type = "";
            string od_rw = "";
            Int32 max = 0;
            Int32 min = 0;
            try
            {
                max = Convert.ToInt32(od.HighLimit);
                min = Convert.ToInt32(od.LowLimit);
            }
            catch
            {
                
            }
            
            int unit_val = 0;
            int data_len = 0;



            switch (od.datatype)
            { 
                case DataType.UNKNOWN:
                    od_type = "OD_DATA_TYPE_UNKNOWN";
                    break;
                case DataType.BOOLEAN:
                    od_type = "OD_DATA_TYPE_BOOLEAN";
                    break;
                case DataType.INTEGER8:
                    od_type = "OD_DATA_TYPE_INT8";
                    break;
                case DataType.INTEGER16:
                    od_type = "OD_DATA_TYPE_INT16";
                    break;
                case DataType.INTEGER32:
                    od_type = "OD_DATA_TYPE_INT32";
                    break;
                case DataType.UNSIGNED8:
                    od_type = "OD_DATA_TYPE_UINT8";
                    break;
                case DataType.UNSIGNED16:
                    od_type = "OD_DATA_TYPE_UINT16";
                    break;
                case DataType.UNSIGNED32:
                    od_type = "OD_DATA_TYPE_UINT32";
                    break;
                case DataType.REAL32:
                    od_type = "OD_DATA_TYPE_REAL32";
                    break;
                default:
                    od_type = "OD_DATA_TYPE_UNKNOWN";
                    break;

            }

            switch (od.accesstype)
            { 
                case AccessType.rw:
                    od_rw = "OD_RW_ACCESS_RW";
                    break;
                case AccessType.ro:
                    od_rw = "OD_RW_ACCESS_RO";
                    break;
                case AccessType.wo:
                    od_rw = "OD_RW_ACCESS_WO";
                    break;
                case AccessType.rwr:
                    od_rw = "OD_RW_ACCESS_RWR";
                    break;
                case AccessType.rww:
                    od_rw = "OD_RW_ACCESS_RWW";
                    break;
                default:
                    od_rw = "OD_RW_ACCESS_C";
                    break;
            }
            
            string od_str = "    {";
            //od_str += String.Format("0x{0},     {1},{2,26},{3,21},      0,      0,      0,{4,12},{5,12},{6,12},\"{7}\", \"\"", indexH, od.Subindex, od_type,od_rw, min,max, 0,od.parameter_name);
            od_str += String.Format("0x{0},     {1},{2,26},{3,4},   \"{4}\"", indexH, od.Subindex, od_type, data_len, od.parameter_name);
            od_str += "},";

            // od_id_list.Add(od_str);

            return od_str;

        }
        private string od_id_name_to_upper(string od_name)
        {
            string od_id="";
            od_id = od_name.ToUpper();
            od_id = od_id.Replace(" ", "_");
            return od_id;
        }
        private string od_id_to_str(ODentry od)
        {
            string od_id_str = "";
            // Device Type TO DEVICE_TYPE
            // 将od id转换为大写 并且 将空格转换为下划线
            // https://api.microsofttranslator.com/v2/Http.svc/Translate?appId=A4D660A48A6A97CCA791C34935E4C02BBB1BEC1C&from=en&to=zh&text=Drive%20Type
            
     
            string ch_str = od.parameter_name;

            string ch_trans = od.parameter_name_trans;

            string od_id = od_id_name_to_upper(ch_str);

            od_id += ",";
            string indexH = $"{od.Index:X4}";


            // 字符串占位 20个字符
            string od_str =String.Format("    OD_ID_{0,-65}//0x{1}  {2}", od_id, indexH, ch_trans);
            
           // od_id_list.Add(od_str);


            
            // 打印
            Console.WriteLine(od_str);
            return od_str;

        }
        private int data_type_to_len(DataType type)
        {
            int len = 0;
            switch (type)
            {
                case DataType.BOOLEAN:
                    len = 1;
                    break;
                case DataType.INTEGER8:
                    len = 1;
                    break;
                case DataType.INTEGER16:
                    len = 2;
                    break;
                case DataType.INTEGER32:
                    len = 4;
                    break;
                case DataType.UNSIGNED8:
                    len = 1;
                    break;
                case DataType.UNSIGNED16:
                    len = 2;
                    break;
                case DataType.UNSIGNED32:
                    len = 4;
                    break;
                case DataType.REAL32:
                    len = 4;
                    break;
                default:
                    len = 0;
                    break;

            }
            return len;
        }
        private string num_to_hex_str(ushort num)
        { 
            return $"{num:X4}";

        }
        #region Prepare
        /// <summary>
        /// Generate ODStorage, ODObjs, ODList, ODDefines and ODCnt entries
        /// </summary>
        /// <param name="ods"></param>
        private void Prepare(EDSsharp eds)
        {
            ODStorageGroups = new List<string>();
            ODStorage_t = new Dictionary<string, List<string>>();
            ODStorage = new Dictionary<string, List<string>>();

            ODObjs_t = new List<string>();
            ODObjs = new List<string>();
            ODList = new List<string>();
            ODDefines = new List<string>();
            ODDefinesLong = new List<string>();
            ODCnt = new Dictionary<string, UInt16>();
            ODArrSize = new Dictionary<string, int>();

            //List<string> mappingErrors = eds.VerifyPDOMapping();
            //if (mappingErrors.Count > 0)
            //    Warnings.AddWarning($"Errors in PDO mappings:\r\n    " + string.Join("\r\n    ", mappingErrors), Warnings.warning_class.WARNING_BUILD);
            libEDSsharp.PDOHelper helper;

            helper = new libEDSsharp.PDOHelper(eds);
            helper.build_PDOlists();
            string pdo_str = "";
            string pdo_enum = "";
            string name_uppper = "";
            string pdo_cfg = "";

            int pos_len = 0;
            ODentry od_temp = new ODentry();
            PDOSlot pdoslot = new PDOSlot();
            // TPDO helper.pdoslots[0]-[3]
            for (int i = 0; i < 4; i++)
            {
                int pos = 0;
                pdoslot = helper.pdoslots[i];
                if (pdoslot.Mapping.Count>0)
                {
                    for (int j = 0; j < pdoslot.Mapping.Count; j++)
                    {
                        od_temp = pdoslot.Mapping[j];

                        //  {OD_ID_TARGET_SPEED_LEFT_RIGHT,0,0},
                        name_uppper = od_id_name_to_upper(od_temp.parameter_name);
                        pdo_str = String.Format("    {{OD_ID_{0},{1},{2}}},", name_uppper, i,pos);
                        
                        pos_len = data_type_to_len(od_temp.datatype);
                        pos += pos_len;

                        tpdo_str_list.Add(pdo_str);

                        pdo_enum = String.Format("TPDO_ID_{0},", name_uppper);
                        tpdo_str_enum.Add(pdo_enum);
                    }                    
                }

                // TPDO CFG 

                pdo_cfg = String.Format("  0x{0},  0x{1},  0x{2},{3,14},{4,14},{5,14},{6,14},{7,14}", num_to_hex_str(pdoslot.ConfigurationIndex), num_to_hex_str(pdoslot.MappingIndex), num_to_hex_str((ushort)pdoslot.COB), pdoslot.transmissiontype, pdoslot.inhibit, pdoslot.eventtimer, pdoslot.syncstart, pdoslot.Mapping.Count);
                tpdo_str_cfg.Add(pdo_cfg);

            }


            // RPDO helper.pdoslots[4]-[7]
            for (int i = 0; i < 4; i++)
            {
                int pos = 0;
                pdoslot = helper.pdoslots[i+4];
                if (pdoslot.Mapping.Count > 0)
                {
                    for (int j = 0; j < pdoslot.Mapping.Count; j++)
                    {
                        od_temp = pdoslot.Mapping[j];

                        //  {OD_ID_TARGET_SPEED_LEFT_RIGHT,0,0},
                        name_uppper = od_id_name_to_upper(od_temp.parameter_name);
                        pdo_str = String.Format("    {{OD_ID_{0},{1},{2}}},", name_uppper, i, pos);

                        pos_len = data_type_to_len(od_temp.datatype);
                        pos += pos_len;

                        rpdo_str_list.Add(pdo_str);

                        pdo_enum = String.Format("RPDO_ID_{0},", name_uppper);
                        rpdo_str_enum.Add(pdo_enum);
                    }
                }

                // TPDO CFG 

                pdo_cfg = String.Format("  0x{0},  0x{1},  0x{2},{3,14},{4,14},{5,14},{6,14},{7,14}", num_to_hex_str(pdoslot.ConfigurationIndex), num_to_hex_str(pdoslot.MappingIndex), num_to_hex_str((ushort)pdoslot.COB), pdoslot.transmissiontype, pdoslot.inhibit, pdoslot.eventtimer, pdoslot.syncstart, pdoslot.Mapping.Count);
                rpdo_str_cfg.Add(pdo_cfg);

            }


            foreach (ODentry od in eds.ods.Values)
            {
                if (od.prop.CO_disabled == true)
                    continue;

                // 0X1400 - 0X2000 跳过
                if (od.Index >= 0X1400 && od.Index < 0x2000)
                {
                    continue;
                }
   
                if (od.Nosubindexes == 0)
                {
                    odentry_list.Add(od);
                }
                else
                {
                    for (ushort i = 0; i < od.Nosubindexes; i++)
                    {
                        if (i != 0)
                            { odentry_list.Add(od.subobjects[i]); }
                        
                    }
                }

            }


            string od_id_str = "";
            foreach (ODentry od in odentry_list)
            {
               
                od_id_str = od_id_to_str(od);
                od_id_list.Add(od_id_str);
            }
            od_id_list.Add("    OD_ID_MAX\r\n");


            // .c struct
            od_id_struct_list.Add("struct micro_canopen_od_items od_items[] = {\r\n");

            //od_id_struct_list.Add("    // [索引]    [子索引]   [数据类型]               [访问权限]         [数据值]  [读取值]  [写入值]  [最小值]  [最大值]  [单位]  [参数名]           [参数描述]");
            od_id_struct_list.Add("    // [索引]    [子索引]   [数据类型]    [数据长度]    [参数描述]");

            foreach (ODentry od in odentry_list)
            {

                od_id_str = od_id_to_struct_str(od);
                od_id_struct_list.Add(od_id_str);
            }
            od_id_struct_list.Add("};");



        }

        /// <summary>
        /// Verify "Count Label" of the object and raise warning if uncorrect.
        /// </summary>
        /// <param name="od"></param>
        /// <param name="indexL"></param>
        /// <param name="indexH"></param>
        /// <param name="countLabel"></param>
        /// <returns></returns>
        private void VerifyCountLabel(ODentry od, ushort indexL, ushort indexH, string countLabel)
        {
            if (od.Index >= indexL && od.Index <= indexH && od.prop.CO_countLabel != countLabel)
            {
                Warnings.AddWarning($"Error in 0x{od.Index:X4}: 'Count Label' must be '{countLabel}'", Warnings.warning_class.WARNING_BUILD);
            }
        }

        /// <summary>
        /// Generate ODStorage and ODObjs entries for VAR
        /// </summary>
        /// <param name="od"></param>
        /// <param name="indexH"></param>
        /// <param name="varName"></param>
        /// <param name="group"></param>
        /// <returns></returns>
        private int Prepare_var(ODentry od, string indexH, string varName, string group)
        {
            DataProperties data = Get_dataProperties(od.datatype, od.defaultvalue, od.prop.CO_stringLengthMin, indexH);
            string attr = Get_attributes(od, data.cTypeMultibyte, data.cTypeString);

            // data storage
            string dataPtr = "NULL";
            if (data.cValue != null)
            {
                ODStorage_t[group].Add($"{data.cType} x{varName}{data.cTypeArray};");
                ODStorage[group].Add($".x{varName} = {data.cValue}");
                dataPtr = $"&{odname}_{group}.x{varName}{data.cTypeArray0}";
            }

            // objects
            ODObjs_t.Add($"OD_obj_var_t o_{varName};");
            ODObjs.Add($"    .o_{varName} = {{");
            ODObjs.Add($"        .dataOrig = {dataPtr},");
            ODObjs.Add($"        .attribute = {attr},");
            ODObjs.Add($"        .dataLength = {data.length}");
            ODObjs.Add($"    }},");

            return 1;
        }

        /// <summary>
        /// Generate ODStorage and ODObjs entries for ARRAY
        /// </summary>
        /// <param name="od"></param>
        /// <param name="indexH"></param>
        /// <param name="varName"></param>
        /// <param name="group"></param>
        /// <returns></returns>
        private int Prepare_arr(ODentry od, string indexH, string varName, string group)
        {
            int subEntriesCount = od.subobjects.Count;
            if (subEntriesCount < 2)
            {
                Warnings.AddWarning($"Error in 0x{indexH}: ARRAY must have minimum two sub entries, not {subEntriesCount}!", Warnings.warning_class.WARNING_BUILD);
                return 0;
            }

            // add size of array
            ODArrSize.Add(indexH, subEntriesCount - 1);

            // prepare and verify each sub element
            string cValue0 = "";
            DataProperties dataElem = new DataProperties();
            string attrElem0 = "";
            string attrElem = "";
            List<string> ODStorageValues = new List<string>();
            UInt16 i = 0;
            foreach (ODentry sub in od.subobjects.Values)
            {
                // If sub datatype is not known, use the od datatype
                DataType dataType = (sub.datatype != DataType.UNKNOWN) ? sub.datatype : od.datatype;

                DataProperties data = Get_dataProperties(dataType, sub.defaultvalue, sub.prop.CO_stringLengthMin, indexH);
                string attr = Get_attributes(sub, data.cTypeMultibyte, data.cTypeString);

                if (sub.Subindex != i)
                    Warnings.AddWarning($"Error in 0x{indexH}: SubIndexes in ARRAY must be in sequence!", Warnings.warning_class.WARNING_BUILD);

                if (i == 0)
                {
                    if (data.cType != "uint8_t" || data.length != 1)
                        Warnings.AddWarning($"Error in 0x{indexH}: Data type in ARRAY in subIndex 0 must be UNSIGNED8, not {sub.datatype}!", Warnings.warning_class.WARNING_BUILD);

                    cValue0 = data.cValue;
                    attrElem0 = attr;
                }
                else
                {
                    if (i == 1)
                    {
                        // First array element. Other array elements must match this elements in data type and attributes
                        dataElem = data;
                        attrElem = attr;
                    }
                    else
                    {
                        if (data.cType != dataElem.cType || data.length != dataElem.length)
                            Warnings.AddWarning($"Error in 0x{indexH}: Data type of elements in ARRAY must be equal!", Warnings.warning_class.WARNING_BUILD);
                        if ((data.cValue == null && dataElem.cValue != null) || (data.cValue != null && dataElem.cValue == null))
                            Warnings.AddWarning($"Error in 0x{indexH}: Default value must be defined on all ARRAY elements or must be undefined on all ARRAY elements!", Warnings.warning_class.WARNING_BUILD);
                        if (attr != attrElem)
                            Warnings.AddWarning($"Error in 0x{indexH}: Attributes of elements in ARRAY must be equal", Warnings.warning_class.WARNING_BUILD);
                    }
                    ODStorageValues.Add($"{data.cValue}");
                }
                i++;
            }
            string dataPtr0 = "NULL";
            string dataPtr = "NULL";
            if (cValue0 != null)
            {
                ODStorage_t[group].Add($"uint8_t x{varName}_sub0;");
                ODStorage[group].Add($".x{varName}_sub0 = {cValue0}");
                dataPtr0 = $"&{odname}_{group}.x{varName}_sub0";
            }
            if (dataElem.cValue != null)
            {
                ODStorage_t[group].Add($"{dataElem.cType} x{varName}[{odname}_CNT_ARR_{indexH}]{dataElem.cTypeArray};");
                ODStorage[group].Add($".x{varName} = {{{string.Join(", ", ODStorageValues)}}}");
                dataPtr = $"&{odname}_{group}.x{varName}[0]{dataElem.cTypeArray0}";
            }

            // sizeof data element inside the array
            string dataElementSizeof = dataElem.cType == "not specified" && dataElem.length == 0
                ? "0" : $"sizeof({dataElem.cType}{dataElem.cTypeArray})";

            // objects
            ODObjs_t.Add($"OD_obj_array_t o_{varName};");
            ODObjs.Add($"    .o_{varName} = {{");
            ODObjs.Add($"        .dataOrig0 = {dataPtr0},");
            ODObjs.Add($"        .dataOrig = {dataPtr},");
            ODObjs.Add($"        .attribute0 = {attrElem0},");
            ODObjs.Add($"        .attribute = {attrElem},");
            ODObjs.Add($"        .dataElementLength = {dataElem.length},");
            ODObjs.Add($"        .dataElementSizeof = {dataElementSizeof}");
            ODObjs.Add($"    }},");

            return subEntriesCount;
        }

        /// <summary>
        /// Generate ODStorage and ODObjs entries for RECORD
        /// </summary>
        /// <param name="od"></param>
        /// <param name="indexH"></param>
        /// <param name="varName"></param>
        /// <param name="group"></param>
        /// <returns></returns>
        private int Prepare_rec(ODentry od, string indexH, string varName, string group)
        {
            int subEntriesCount = od.subobjects.Count;
            if (subEntriesCount < 2)
            {
                Warnings.AddWarning($"Error in 0x{indexH}: RECORD must have minimum two sub entries, not {subEntriesCount}!", Warnings.warning_class.WARNING_BUILD);
                return 0;
            }

            List<string> subODStorage_t = new List<string>();
            List<string> subODStorage = new List<string>();

            ODObjs_t.Add($"OD_obj_record_t o_{varName}[{subEntriesCount}];");
            ODObjs.Add($"    .o_{varName} = {{");

            foreach (ODentry sub in od.subobjects.Values)
            {
                DataProperties data = Get_dataProperties(sub.datatype, sub.defaultvalue, sub.prop.CO_stringLengthMin, indexH);
                string attr = Get_attributes(sub, data.cTypeMultibyte, data.cTypeString);

                if (sub.Subindex == 0 && (data.cType != "uint8_t" || data.length != 1))
                    Warnings.AddWarning($"Error in 0x{indexH}: Data type in RECORD, subIndex 0 must be UNSIGNED8, not {sub.datatype}!", Warnings.warning_class.WARNING_BUILD);

                string subcName = Make_cname(sub.parameter_name);
                string dataPtr = "NULL";
                if (data.cValue != null)
                {
                    subODStorage_t.Add($"{data.cType} {subcName}{data.cTypeArray};");
                    subODStorage.Add($".{subcName} = {data.cValue}");
                    dataPtr = $"&{odname}_{group}.x{varName}.{subcName}{data.cTypeArray0}";
                }
                ODObjs.Add($"        {{");
                ODObjs.Add($"            .dataOrig = {dataPtr},");
                ODObjs.Add($"            .subIndex = {sub.Subindex},");
                ODObjs.Add($"            .attribute = {attr},");
                ODObjs.Add($"            .dataLength = {data.length}");
                ODObjs.Add($"        }},");

            }
            // remove last ',' and add closing bracket
            string s = ODObjs[ODObjs.Count - 1];
            ODObjs[ODObjs.Count - 1] = s.Remove(s.Length - 1);
            ODObjs.Add($"    }},");

            if (subODStorage_t.Count > 0)
            {
                ODStorage_t[group].Add($"struct {{\n        {string.Join("\n        ", subODStorage_t)}\n    }} x{varName};");
                ODStorage[group].Add($".x{varName} = {{\n        {string.Join(",\n        ", subODStorage)}\n    }}");
            }

            return subEntriesCount;
        }
        #endregion

        #region Exporters

        /// <summary>
        /// Export the header file
        /// </summary>
        /// <param name="folderpath"></param>
        /// <param name="filename"></param>
        /// <param name="gitVersion"></param>
        /// <param name="fi"></param>
        /// <param name="di"></param>
        private void Export_h(string folderpath, string filename, string gitVersion, EDSsharp eds)
        {
            string temp = "";

            if (filename == "")
                filename = "OD";

            StreamWriter file = new StreamWriter(folderpath + Path.DirectorySeparatorChar + filename + ".h");
            file.NewLine = "\n";

            file.WriteLine(string.Format(
@"/*******************************************************************************
    CANopen Object Dictionary definition for CANopenNode V5

    This file was automatically generated by CANopenEditor {0}

    https://github.com/CANopenNode/CANopenNode
    https://github.com/CANopenNode/CANopenEditor

    DON'T EDIT THIS FILE MANUALLY !!!!
********************************************************************************

    File info:
        File Names:   {1}.h; {1}.c
        Project File: {2}
        File Version: {3}

        Created:      {4}
        Created By:   {5}
        Modified:     {6}
        Modified By:  {7}

    Device Info:
        Vendor Name:  {8}
        Vendor ID:    {9}
        Product Name: {10}
        Product ID:   {11}

        Description:  {12}
*******************************************************************************/",
            gitVersion, odname,
            Path.GetFileName(eds.projectFilename), eds.fi.FileVersion,
            eds.fi.CreationDateTime, eds.fi.CreatedBy, eds.fi.ModificationDateTime, eds.fi.ModifiedBy,
            eds.di.VendorName, eds.di.VendorNumber, eds.di.ProductName, eds.di.ProductNumber,
            eds.fi.Description));

            file.WriteLine(string.Format(@"
#ifndef {0}_H
#define {0}_H

#include ""micro_canopen.h""",
            odname));

            file.WriteLine("enum \r\n{\r\n");            
            foreach(string od_id in od_id_list)
            {
                file.WriteLine(od_id);
            }

            file.WriteLine("};");





            file.WriteLine("/*--------------------------------------------- RPOD_ID -----------------------------------*/");
            file.WriteLine("enum{");
            foreach (string str_val in rpdo_str_enum)
            {
                temp = "    " + str_val;
                file.WriteLine(temp);
            }
            file.WriteLine("};");


            file.WriteLine("/*--------------------------------------------- TPOD_ID -----------------------------------*/");
            file.WriteLine("enum{");
            foreach (string str_val in tpdo_str_enum)
            {
                temp = "    " + str_val;
                file.WriteLine(temp);
            }
            file.WriteLine("};");



            file.WriteLine(string.Format("#define RPDO_ITEMS_NUM  {0}\r\n#define RPDO_CFG_NUM 4\r\n#define TPDO_ITEMS_NUM  {1}\r\n#define TPDO_CFG_NUM 4", rpdo_str_list.Count, tpdo_str_list.Count));

            string ext = @"
extern struct micro_canopen_od_items od_items[];

extern struct micro_canopen_od_pdo_itmes_t tpdo_items[];
extern struct micro_canopen_pdo_cfg_t tpdo_cfg[];

extern struct micro_canopen_od_pdo_itmes_t rpdo_items[];
extern struct micro_canopen_pdo_cfg_t rpdo_cfg[];
";

            

            file.WriteLine(ext);

            file.WriteLine("#endif\r\n");

            // 

            file.Close();
        }


        /// <summary>
        /// Export the c file
        /// </summary>
        /// <param name="folderpath"></param>
        /// <param name="filename"></param>
        /// <param name="gitVersion"></param>
        private void Export_c(string folderpath, string filename, string gitVersion, EDSsharp eds)
            {
            string temp = "";

            if (filename == "")
                filename = "OD";



            StreamWriter file = new StreamWriter(folderpath + Path.DirectorySeparatorChar + filename + ".c");
            file.NewLine = "\n";


            file.WriteLine(@"# include ""micro_canopen_od.h""");

            foreach (string str_val in od_id_struct_list)
            {
                file.WriteLine(str_val);
            }

            file.WriteLine("/*--------------------------------------------- RPDO RPDO -----------------------------------*/");
            file.WriteLine("struct micro_canopen_od_pdo_itmes_t rpdo_items[]= {");
            foreach (string str_val in rpdo_str_list)
            {
                file.WriteLine(str_val);
            }
            file.WriteLine("};");

            file.WriteLine("struct micro_canopen_pdo_cfg_t rpdo_cfg[] = {");
            foreach (string str_val in rpdo_str_cfg)
            {
                temp = "    {" + str_val + "},";
                file.WriteLine(temp);
            }

            file.WriteLine("};");



            // TPDO
            file.WriteLine("/*--------------------------------------------- TPDO RPDO -----------------------------------*/");
            file.WriteLine("struct micro_canopen_od_pdo_itmes_t tpdo_items[]= {");
            foreach (string str_val in tpdo_str_list)
            {
                file.WriteLine(str_val);
            }
            file.WriteLine("};");

            file.WriteLine("struct micro_canopen_pdo_cfg_t tpdo_cfg[] = {");
            foreach (string str_val in tpdo_str_cfg)
            {
                temp = "    {" + str_val + "},";
                file.WriteLine(temp);
            }

            file.WriteLine("};");




            //


            file.Close();
        }
        #endregion

        #region helper_functions

        /// <summary>
        /// Take a paramater name from the object dictionary and make it acceptable
        /// for use in c variables/structs etc
        /// </summary>
        /// <param name="name">string, name to convert</param>
        /// <returns>string</returns>
        protected static string Make_cname(string name)
        {
            if (name == null || name == "")
                return "";

            // split string to tokens, separated by non-word characters. Remove any empty strings
            var tokens = Regex.Split(name.Replace('-', '_'), @"[\W]+").Where(s => s != String.Empty);

            string output = "";
            char prev = ' ';
            foreach (string tok in tokens)
            {
                char first = tok[0];

                if (Char.IsUpper(prev) && Char.IsUpper(first))
                {
                    // add underscore, if we have two upper-case words
                    output += "_";
                }

                if (tok.Length > 1 && Char.IsLetter(first))
                {
                    // all tokens except the first start with uppercase letter
                    output += Char.ToUpper(first) + tok.Substring(1);
                }
                else
                {
                    // use token as is and handle what the start of the output looks like outside of the loop 
                    output += tok;
                }

                prev = tok[tok.Length - 1];
            }

            if (Char.IsDigit(output[0]))
            {
                // output that starts with a digit needs a starting underscore 
                output = "_" + output;
            }
            else if (output.Length > 1)
            {
                // output that doesnt start with all-cap-words should have word start with a lower case character
                if (Char.IsLetter(output[0]) && Char.IsLower(output[1]))
                    output = Char.ToLowerInvariant(output[0]) + output.Substring(1);
            }
            else
            {
                // single character output
                output = output.ToLowerInvariant();
            }

            return output;
        }

        /// <summary>
        /// Return from Get_dataProperties
        /// </summary>
        private class DataProperties
        {
            public string cType = "not specified";
            public string cTypeArray = "";
            public string cTypeArray0 = "";
            public bool cTypeMultibyte = false;
            public bool cTypeString = false;
            public UInt32 length = 0;
            public string cValue = null;
        }

        /// <summary>
        /// Get the correct c data type, length and default value, based on CANopen data type
        /// </summary>
        /// <param name="dataType"></param>
        /// <param name="defaultvalue"></param>
        /// <param name="stringLength"></param>
        /// <param name="indexH"></param>
        /// <returns>Structure filled with data</returns>
        private DataProperties Get_dataProperties(DataType dataType, string defaultvalue, UInt32 stringLength, string indexH)
        {
            DataProperties data = new DataProperties();
            int nobase = 10;
            bool valueDefined = true;
            if (defaultvalue == null || defaultvalue == "")
                valueDefined = false;
            else if (dataType != DataType.VISIBLE_STRING && dataType != DataType.UNICODE_STRING && dataType != DataType.OCTET_STRING)
            {
                defaultvalue = defaultvalue.Trim();

                if (defaultvalue.Contains("$NODEID"))
                {
                    defaultvalue = defaultvalue.Replace("$NODEID", "");
                    defaultvalue = defaultvalue.Replace("+", "");
                    defaultvalue = defaultvalue.Trim();
                    if (defaultvalue == "")
                        defaultvalue = "0";
                }

                String pat = @"^0[xX][0-9a-fA-FUL]+";
                Regex r = new Regex(pat, RegexOptions.IgnoreCase);
                Match m = r.Match(defaultvalue);
                if (m.Success)
                {
                    nobase = 16;
                    defaultvalue = defaultvalue.Replace("U", "");
                    defaultvalue = defaultvalue.Replace("L", "");
                }

                pat = @"^0[0-7]+";
                r = new Regex(pat, RegexOptions.IgnoreCase);
                m = r.Match(defaultvalue);
                if (m.Success)
                {
                    nobase = 8;
                }
            }

            try
            {
                bool signedNumber = false;
                bool unsignedNumber = false;

                switch (dataType)
                {
                    case DataType.BOOLEAN:
                        data.length = 1;
                        data.cType = "bool_t";
                        if (valueDefined)
                        {
                            data.cValue = (defaultvalue.ToLower() == "false" || defaultvalue == "0") ? "false" : "true";
                        }
                        break;
                    case DataType.INTEGER8:
                        data.length = 1;
                        data.cType = "int8_t";
                        if (valueDefined)
                        {
                            data.cValue = $"{Convert.ToSByte(defaultvalue, nobase)}";
                        }
                        break;
                    case DataType.INTEGER16:
                        data.length = 2;
                        data.cType = "int16_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = $"{Convert.ToInt16(defaultvalue, nobase)}";
                        }
                        break;
                    case DataType.INTEGER32:
                        data.length = 4;
                        data.cType = "int32_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = $"{Convert.ToInt32(defaultvalue, nobase)}";
                        }
                        break;
                    case DataType.INTEGER64:
                        data.length = 8;
                        data.cType = "int64_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = $"{Convert.ToInt64(defaultvalue, nobase)}";
                        }
                        break;

                    case DataType.UNSIGNED8:
                        data.length = 1;
                        data.cType = "uint8_t";
                        if (valueDefined)
                        {
                            data.cValue = String.Format("0x{0:X2}", Convert.ToByte(defaultvalue, nobase));
                        }
                        break;
                    case DataType.UNSIGNED16:
                        data.length = 2;
                        data.cType = "uint16_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = String.Format("0x{0:X4}", Convert.ToUInt16(defaultvalue, nobase));
                        }
                        break;
                    case DataType.UNSIGNED32:
                        data.length = 4;
                        data.cType = "uint32_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = String.Format("0x{0:X8}", Convert.ToUInt32(defaultvalue, nobase));
                        }
                        break;
                    case DataType.UNSIGNED64:
                        data.length = 8;
                        data.cType = "uint64_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = String.Format("0x{0:X16}", Convert.ToUInt64(defaultvalue, nobase));
                        }
                        break;

                    case DataType.REAL32:
                        data.length = 4;
                        data.cType = "float32_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = defaultvalue;
                        }
                        break;
                    case DataType.REAL64:
                        data.length = 8;
                        data.cType = "float64_t";
                        data.cTypeMultibyte = true;
                        if (valueDefined)
                        {
                            data.cValue = defaultvalue;
                        }
                        break;

                    case DataType.DOMAIN:
                        // keep default values (0 and null)
                        break;

                    case DataType.VISIBLE_STRING:
                        data.cTypeString = true;
                        if (valueDefined || stringLength > 0)
                        {
                            List<string> chars = new List<string>();
                            UInt32 len = 0;

                            if (valueDefined)
                            {
                                UTF8Encoding utf8 = new UTF8Encoding();
                                Byte[] encodedBytes = utf8.GetBytes(defaultvalue);
                                foreach (Byte b in encodedBytes)
                                {
                                    if ((char)b == '\'')
                                        chars.Add("'\\''");
                                    else if (b >= 0x20 && b < 0x7F)
                                        chars.Add($"'{(char)b}'");
                                    else if (b <= 0x7F)
                                        chars.Add($"0x{b:X2}");
                                    else
                                        chars.Add($"(char)0x{b:X2}");
                                    len++;
                                }
                            }
                            /* fill unused bytes with nulls */
                            for (; len < stringLength; len++)
                            {
                                chars.Add("0");
                            }

                            // extra string terminator
                            chars.Add("0");

                            data.length = len;
                            data.cType = "char";
                            data.cTypeArray = $"[{len + 1}]";
                            data.cTypeArray0 = "[0]";
                            data.cValue = $"{{{string.Join(", ", chars)}}}";
                        }
                        break;

                    case DataType.OCTET_STRING:
                        defaultvalue = defaultvalue.Trim();
                        if (defaultvalue == "")
                            valueDefined = false;
                        if (valueDefined || stringLength > 0)
                        {
                            List<string> bytes = new List<string>();
                            UInt32 len = 0;

                            if (valueDefined)
                            {
                                string[] strBytes = defaultvalue.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                                foreach (string s in strBytes)
                                {
                                    bytes.Add(String.Format("0x{0:X2}", Convert.ToByte(s, 16)));
                                    len++;
                                }
                            }
                            for (; len < stringLength; len++)
                            {
                                bytes.Add("0x00");
                            }

                            data.length = len;
                            data.cType = "uint8_t";
                            data.cTypeArray = $"[{len}]";
                            data.cTypeArray0 = "[0]";
                            data.cValue = $"{{{string.Join(", ", bytes)}}}";
                        }
                        break;
                    case DataType.UNICODE_STRING:
                        data.cTypeString = true;
                        if (valueDefined || stringLength > 0)
                        {
                            List<string> words = new List<string>();
                            UInt32 len = 0;

                            if (valueDefined)
                            {
                                Encoding unicode = Encoding.Unicode;
                                Byte[] encodedBytes = unicode.GetBytes(defaultvalue);
                                for (UInt32 i = 0; i < encodedBytes.Length; i += 2)
                                {
                                    UInt16 val = (ushort)(encodedBytes[i] | (UInt16)encodedBytes[i+1] << 8);
                                    words.Add(String.Format("0x{0:X4}", val));
                                    len++;
                                }
                            }
                            for (; len < stringLength; len++)
                            {
                                words.Add("0x0000");
                            }

                            // extra string terminator
                            words.Add("0x0000");

                            data.length = len * 2;
                            data.cType = "uint16_t";
                            data.cTypeArray = $"[{len + 1}]";
                            data.cTypeArray0 = "[0]";
                            data.cValue = $"{{{string.Join(", ", words)}}}";
                        }
                        break;

                    case DataType.INTEGER24:
                        data.length = 3;
                        signedNumber = true;
                        break;
                    case DataType.INTEGER40:
                        data.length = 5;
                        signedNumber = true;
                        break;
                    case DataType.INTEGER48:
                        data.length = 6;
                        signedNumber = true;
                        break;
                    case DataType.INTEGER56:
                        data.length = 7;
                        signedNumber = true;
                        break;

                    case DataType.UNSIGNED24:
                        data.length = 3;
                        unsignedNumber = true;
                        break;
                    case DataType.UNSIGNED40:
                        data.length = 5;
                        unsignedNumber = true;
                        break;
                    case DataType.UNSIGNED48:
                    case DataType.TIME_OF_DAY:
                    case DataType.TIME_DIFFERENCE:
                        data.length = 6;
                        unsignedNumber = true;
                        break;
                    case DataType.UNSIGNED56:
                        data.length = 7;
                        unsignedNumber = true;
                        break;

                    default:
                        Warnings.AddWarning($"Error in 0x{indexH}: Unknown dataType: {dataType}", Warnings.warning_class.WARNING_BUILD);
                        break;
                }

                if (valueDefined && (signedNumber || unsignedNumber))
                {
                    // write default value as a sequence of bytes, like "{0x56, 0x34, 0x12}"
                    ulong value = signedNumber ? (ulong)Convert.ToInt64(defaultvalue, nobase) : Convert.ToUInt64(defaultvalue, nobase);
                    List<string> bytes = new List<string>();
                    for (UInt32 i = 0; i < data.length; i++)
                    {
                        bytes.Add(String.Format("0x{0:X2}", value & 0xFF));
                        value >>= 8;
                    }
                    if (value > 0)
                        Warnings.AddWarning($"Error in 0x{indexH}: Overflow error in default value {defaultvalue} of type {dataType}", Warnings.warning_class.WARNING_BUILD);
                    else
                    {
                        data.cType = "uint8_t";
                        data.cTypeArray = $"[{data.length}]";
                        data.cTypeArray0 = "[0]";
                        data.cValue = $"{{{string.Join(", ", bytes)}}}";
                    }
                }
            }
            catch (Exception)
            {
                Warnings.AddWarning($"Error in 0x{indexH}: Error converting default value {defaultvalue} to type {dataType}", Warnings.warning_class.WARNING_BUILD);
            }

            return data;
        }

        /// <summary>
        /// Get attributes from OD entry or sub-entry
        /// </summary>
        /// <param name="od"></param>
        /// <param name="cTypeMultibyte"></param>
        /// <param name="cTypeString"></param>
        /// <returns></returns>
        private string Get_attributes(ODentry od, bool cTypeMultibyte, bool cTypeString)
        {
            List<string> attributes = new List<string>();

            switch (od.AccessSDO())
            {
                case AccessSDO.ro:
                    attributes.Add("ODA_SDO_R");
                    break;
                case AccessSDO.wo:
                    attributes.Add("ODA_SDO_W");
                    break;
                case AccessSDO.rw:
                    attributes.Add("ODA_SDO_RW");
                    break;
            }

            switch (od.AccessPDO())
            {
                case AccessPDO.r:
                    attributes.Add("ODA_RPDO");
                    break;
                case AccessPDO.t:
                    attributes.Add("ODA_TPDO");
                    break;
                case AccessPDO.tr:
                    attributes.Add("ODA_TRPDO");
                    break;
            }

            switch (od.prop.CO_accessSRDO)
            {
                case AccessSRDO.rx:
                    attributes.Add("ODA_RSRDO");
                    break;
                case AccessSRDO.tx:
                    attributes.Add("ODA_TSRDO");
                    break;
                case AccessSRDO.trx:
                    attributes.Add("ODA_TRSRDO");
                    break;
            }

            if (cTypeMultibyte)
                attributes.Add("ODA_MB");

            if (cTypeString)
                attributes.Add("ODA_STR");

            if (attributes.Count == 0) attributes.Add("0");

            return string.Join(" | ", attributes);
        }
        #endregion
    }
}
