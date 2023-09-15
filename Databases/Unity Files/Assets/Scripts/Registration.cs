using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Registration : MonoBehaviour
{
    [SerializeField] private Button registerbutton;
    
    public void CallRegister()
    {
        StartCoroutine(Register());
    }
    
    private IEnumerator Register()
    {
        WWWForm form = new WWWForm();
        form.AddField("name", "Emre");
        form.AddField("password", "3131");
        WWW www = new WWW("http://localhost/sqlconnect/register.php", form);
        yield return www;
        if (www.text == "0")
        {
            Debug.Log("User created successfully");
        }
        else
        {
            Debug.Log("User created unsuccessfully. Error #" + www.text);
        }
    }
}
