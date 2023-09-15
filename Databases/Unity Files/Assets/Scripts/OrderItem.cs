using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class OrderItem : MonoBehaviour
{
    public TextMeshProUGUI idText;    
    public TextMeshProUGUI customerNameText;    
    public TextMeshProUGUI gameNameText;    
    public TextMeshProUGUI gamePriceText;    
    public TextMeshProUGUI addressText;    
    public TextMeshProUGUI dateText;    
    public TextMeshProUGUI billIdText;    
    public TextMeshProUGUI billAddressText;    
    public TextMeshProUGUI billDueDateText;    

    public int index;
    public int id;
    public string customerName;
    public string gameName;
    public int gamePrice;
    public string address;
    public string date;
    public int billId;
    public string billAddress;
    public string billDueDate;

    public void SetTexts(int index, int id, string customerName, string gameName, int gamePrice, string address, string date, int billId, string billAddress, string billDueDate)
    {
        this.index = index;
        this.id = id;
        this.customerName = customerName;
        this.gameName = gameName;
        this.gamePrice = gamePrice;
        this.address = address;
        this.date = date;
        this.billId = billId;
        this.billAddress = billAddress;
        this.billDueDate = billDueDate;

        idText.text = id.ToString();
        customerNameText.text = customerName;
        gameNameText.text = gameName;
        gamePriceText.text = gamePrice.ToString();
        addressText.text = address;
        dateText.text = date;
        billIdText.text = this.billId.ToString();
        billAddressText.text = billAddress;
        billDueDateText.text = billDueDate;
    }
}
