using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class NotificationItem : MonoBehaviour
{
    public TextMeshProUGUI contentText;
    public TextMeshProUGUI gameNameText;


    public int index;
    public string content;
    public string gameName;


    public void SetTexts(string content, string gameName)
    {
        this.content = content;
        this.gameName = gameName;

        contentText.text = content;
        gameNameText.text = gameName;
    }
}