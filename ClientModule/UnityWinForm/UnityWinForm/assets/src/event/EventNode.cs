using System;
using System.Diagnostics;
using System.Collections.Generic;

/// <summary>
/// <para> 事件节点类(非线程安全) </para>
/// <para> 基于监听者模式,遍历所有事件节点,派发消息至所有节点绑定的所有监听者 </para> 
/// </summary>

public class EventNode
{
    private string m_strName    = "";       // 当前节点名
    private bool m_bValid       = true;     // 当前节点是否有效

    /// <summary> 这个消息节点的子节点 </summary>
    protected List<EventNode> mNodeList = new List<EventNode>();

    /// <summary> 所有消息 以及消息监听者的集合 </summary>
    protected Dictionary<int, List<IEventListener>> mListeners = new Dictionary<int, List<IEventListener>>();
    
    public EventNode()
    { }

    /// <summary> 清理所有 </summary>
    ~EventNode()
    {
        mListeners.Clear();
        mNodeList.Clear();
    }

    /// <summary> 当前节点是否有效 </summary>
    public virtual bool IsValid
    {
        set { this.m_bValid = value; }

        get { return this.m_bValid; }
    }

    /// <summary> 当前节点名 </summary>
    public virtual string Name
    {
        set { this.m_strName = value; }

        // 若未设置节点名,则默认为当前空间名.类名
        get { return this.m_strName.Equals("") ? this.GetType().ToString() : m_strName; }
    }

    /// <summary> 节点的优先级 </summary>
    public virtual int EventNodePriority { get { return 0; } }
    
    /// <summary> 挂载一个节点到这个节点上 </summary>
    public bool AttachEventNode(EventNode node)
    {
        bool bRes = false;
        if (node != null)
        {
            if (mNodeList.Contains(node))
            {
                bRes = false;
                Debug.Print("EventNode - AttachEventNode Failed, this Node is Already Exist! [node = {0}]", this.Name);
            }
            else
            {
                // 这个节点要放的位置
                int pos = 0;
                for (int i = 0; i < mNodeList.Count; ++i, ++pos)
                {
                    if (node.EventNodePriority > mNodeList[i].EventNodePriority)
                    {
                        break;
                    }
                }

                mNodeList.Insert(pos, node);
                bRes = true;
            }
        }

        return bRes;
    }
        
    /// <summary> 卸载一个节点 </summary>
    public bool DetachEventNode(EventNode node)
    {
        if (mNodeList.Contains(node))
        {
            mNodeList.Remove(node);
            return true;
        }
        return false;
    }

    /// <summary> 挂载一个消息监听者到当前的节点上 </summary>
    public bool AttachEventListener(int id, IEventListener listener)
    {
        bool bRes = false;
        if (listener != null)
        {
            if (!mListeners.ContainsKey(id))
            {
                mListeners[id] = new List<IEventListener>() { listener };
            }
            else
            {
                // 若该监听这列表里已存在,则跳过
                for (int i = 0; i < mListeners[id].Count; ++i)
                {
                    if (mListeners[id][i].GetHashCode() == listener.GetHashCode())
                    {
                        return true;
                    }
                }

                // 插入至监听列表中
                int pos = 0;
                for (int i = 0; i < mListeners[id].Count; i++)
                {
                    if (listener.EventProority > mListeners[id][i].EventProority)
                    {
                        break;
                    }
                    pos++;
                }
                mListeners[id].Insert(pos, listener);
            }
            bRes = true;
        }

        return bRes;
    }

    /// <summary> 卸载一个消息监听者 </summary>
    public bool DetachEventListener(int id, IEventListener listener)
    {
        if (mListeners.ContainsKey(id) && mListeners[id].Contains(listener))
        {
            mListeners[id].Remove(listener);
            return true;
        }

        return false;
    }

    /// <summary> 发送消息 </summary>
    public void SendEvent(int id, object param1 = null, object param2 = null)
    {
        DispatchEvent(id, param1, param2);
    }

    /// <summary> 派发消息给所有节点 </summary>
    protected bool DispatchEvent(int key, object param1, object param2)
    {
        for (int i = 0; i < mNodeList.Count; ++i)
        {
            if (mNodeList[i].DispatchEvent(key, param1, param2)) return true;
        }
        return TriggerEvent(key, param1, param2);
    }

    /// <summary> 派发消息给该节点的所有监听者 </summary>
    private bool TriggerEvent(int id, object param1, object param2)
    {
        bool bRes = false;

        if (this.IsValid)
        {
            if (mListeners.ContainsKey(id))
            {
                //这个消息的所有监听者
                List<IEventListener> listener = mListeners[id];
                for (int i = 0; i < listener.Count; i++)
                {
                    if (listener[i].HandleEvent(id, param1, param2))
                    {
                        bRes = true;
                        break;
                    }
                }
            }
        }

        return bRes;
    }
}
