using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Web;
using System.Net;
using Flurl;
using Flurl.Http;


namespace ODEditor
{
    class Translation
    {
        public async Task<string> TransAsync(string q)
        {
            // 原文
            // string q = "apple";
            // 源语言
            string from = "en";
            // 目标语言
            string to = "zh";
            // 改成您的APP ID
            string appId = "20220114001055871";
            Random rd = new Random();
            string salt = rd.Next(100000).ToString();
            // 改成您的密钥
            string secretKey = "pxxr7dpLeHKp2DM5Xrxq";
            string sign = EncryptString(appId + q + salt + secretKey);
            string url = "http://api.fanyi.baidu.com/api/trans/vip/translate?";
            url += "q=" + HttpUtility.UrlEncode(q);
            url += "&from=" + from;
            url += "&to=" + to;
            url += "&appid=" + appId;
            url += "&salt=" + salt;
            url += "&sign=" + sign;

            string retString = await url.GetStringAsync();

            //HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
            //request.Method = "GET";
            //request.ContentType = "text/html;charset=UTF-8";
            //request.UserAgent = null;
            //request.Timeout = 6000;
            //HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            //Stream myResponseStream = response.GetResponseStream();
            //StreamReader myStreamReader = new StreamReader(myResponseStream, Encoding.GetEncoding("utf-8"));
            //string retString = myStreamReader.ReadToEnd();

            //myStreamReader.Close();
            //myResponseStream.Close();

            if (retString.IndexOf("dst") == -1)
            {
                return "";
            }
            string dst = retString.Substring(retString.IndexOf("dst") + 6);
            dst = dst.Substring(0, dst.IndexOf("\""));
            string ret = Regex.Unescape(dst);
            return ret;

            // Console.WriteLine(retString);
            //Console.ReadLine();
        }

        // 计算MD5值
        public static string EncryptString(string str)
        {
            MD5 md5 = MD5.Create();
            // 将字符串转换成字节数组
            byte[] byteOld = Encoding.UTF8.GetBytes(str);
            // 调用加密方法
            byte[] byteNew = md5.ComputeHash(byteOld);
            // 将加密结果转换为字符串
            StringBuilder sb = new StringBuilder();
            foreach (byte b in byteNew)
            {
                // 将字节转换成16进制表示的字符串，
                sb.Append(b.ToString("x2"));
            }
            // 返回加密的字符串
            return sb.ToString();
        }

        internal object TransAsync()
        {
            throw new NotImplementedException();
        }
    }
}
