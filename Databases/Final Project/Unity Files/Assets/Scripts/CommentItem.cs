using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class CommentItem : MonoBehaviour
{
    public TextMeshProUGUI customerNameText;
    public TextMeshProUGUI contentText;


    public int index;
    public string customerName;
    public string content;


    public void SetTexts(string customerName, string content)
    {
        this.customerName = customerName;
        this.content = content;

        customerNameText.text = customerName;
        contentText.text = content;
    }
}