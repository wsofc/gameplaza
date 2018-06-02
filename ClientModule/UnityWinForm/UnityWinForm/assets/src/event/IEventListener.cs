
public interface IEventListener
{
    /// <summary>
    /// return 是否中断消息传递(false:继续传递,低于该优先级的监听者可以继续接收该消息)
    /// </summary>
    bool HandleEvent(int id, object param1 = null, object param2 = null);

    /// <summary> 消息派发的优先级别(越大越优先) </summary>
    int EventProority { get; }
}
