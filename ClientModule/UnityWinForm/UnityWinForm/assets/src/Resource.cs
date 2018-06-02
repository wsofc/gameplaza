using System;
using System.Drawing;
using System.Diagnostics;
using System.Windows.Forms;
using System.Collections.Generic;

public class Resource
{
    private static Resource g_Instance                      = null;     // 单例
    
    private string m_strGameResPath                         = "";       // 游戏资源路径
    private Dictionary<string, Image> m_vImgMap             = null;     // 图片容器
    private Dictionary<string, ImageList> m_vImageListMap   = null;     // 图集容器
    
    public string GameRespath { get { return this.m_strGameResPath; } }

    private Resource()
    {
        this.m_vImgMap = new Dictionary<string, Image>();
        this.m_vImageListMap = new Dictionary<string, ImageList>();
    }

    ~Resource() { }

    public static Resource GetInstance()
    {
        if (Resource.g_Instance == null)
        {
            Resource.g_Instance = new Resource();
        }
        return Resource.g_Instance;
    }

    public void Init()
    {
        string strTmp = String.Format("{0}\\{1}\\{2}_Data\\Resources\\ShellForm\\", System.Windows.Forms.Application.StartupPath, Protocol.GameFileName, Protocol.GameFileName);
        this.m_strGameResPath = System.IO.Path.GetFullPath(strTmp);
    }

    // 加载嵌入资源
    public Image LoadImgFromAssembly(string filename)
    {
        Image img = null;
        String path = filename;
        path = path.Replace("\\", ".");
        path = path.Replace("/", ".");
        path = String.Format("UnityWinForm.assets.img.{0}", path);
        
        System.Reflection.Assembly asm = System.Reflection.Assembly.GetExecutingAssembly();
        System.IO.Stream imgStream = asm.GetManifestResourceStream(path);
        if (imgStream != null)
        {
            img = Image.FromStream(imgStream);
        }
        return img;
    }
    
    public Image LoadImgFromGameRes(string filename)
    {
        string path = System.IO.Path.GetFullPath(this.GameRespath + filename);
        return this.LoadImgFromFile(path);
    }

    Image LoadImgFromFile(string filename)
    {
        Image img = null;

        try
        {
            if (this.m_vImgMap.ContainsKey(filename))
            {
                img = this.m_vImgMap[filename];
            }
            else
            {
                string strFullPath = System.IO.Path.GetFullPath(filename);
                img = Image.FromFile(strFullPath);
                if (img != null)
                {
                    this.m_vImgMap[filename] = img;
                }
            }
        }
        catch (System.OutOfMemoryException ex)
        {
            // 该文件没有有效的图像格式。 - 或 - GDI+ 不支持文件的像素格式
            Debug.Print("该文件没有有效的图像格式:" + ex.Message);
        }
        catch (System.IO.FileNotFoundException ex)
        {
            // 指定的文件不存在
            Debug.Print("指定的文件不存在:" + ex.Message);
        }
        catch (System.ArgumentException ex)
        {
            // filename 为 System.Uri
            Debug.Print("filename 为 System.Uri: " + ex.Message);
        }

        return img;
    }

    /// <summary> 加载图集(以一个文件夹的资源为单位) </summary>
    public ImageList LoadImageList(string imglistname, string foldername)
    {
        ImageList imglist = null;
        if (this.m_vImageListMap.ContainsKey(imglistname))
        {
            imglist = this.m_vImageListMap[imglistname];
        }
        else
        {
            System.IO.DirectoryInfo folder = new System.IO.DirectoryInfo(foldername);
            if (folder != null)
            {
                imglist = new ImageList();
                foreach (System.IO.FileInfo file in folder.GetFiles("*.png"))
                {
                    Image img = this.LoadImgFromFile(foldername + "\\" + file.Name);
                    if (img != null)
                    {
                        imglist.Images.Add(img);
                    }
                }
                this.m_vImageListMap[imglistname] = imglist;
            }
        }

        return imglist;
    }
}
