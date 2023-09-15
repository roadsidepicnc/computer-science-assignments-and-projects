using UnityEngine;

public class Singleton <T> : MonoBehaviour where T : Singleton<T>
{
    public static T instance;
    [SerializeField] private bool isPersistent;
    
    private void Awake()
    {
        if (isPersistent)
        {
            if (instance == null)
            {
                instance = this as T;
                DontDestroyOnLoad(gameObject);
            }
            else
            {
                Destroy(gameObject);
            }
            
            
        }
        else
        {
            instance = this as T;
        }
    }
}
