using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class GameListing : MonoBehaviour
{
    public TextMeshProUGUI nameText;    
    public TextMeshProUGUI basePriceText;    
    public TextMeshProUGUI discountPercentageText;    
    public TextMeshProUGUI netPriceText;    
    public TextMeshProUGUI releaseYearText;    
    public TextMeshProUGUI publisherText;    
    public TextMeshProUGUI ratingText;
    public TextMeshProUGUI categoryText;
    public Button buyButton;
    public Button commensButton;

    public int index;
    public int id;
    public string gameName;
    public int basePrice;
    public int discountPercentage;
    public int netPrice;
    public int releaseYear;
    public string publisher;
    public int rating;
    public List<string> categories;

    public void SetTexts(int id, string gameName, int basePrice, int discountPercentage, int netPrice, int releaseYear, string publisher, int rating, string category)
    {
        this.id = id;
        this.gameName = gameName;
        this.basePrice = basePrice;
        this.discountPercentage = discountPercentage;
        this.netPrice = netPrice;
        this.releaseYear = releaseYear;
        this.publisher = publisher;
        this.rating = rating;
        categories.Add(category);

        nameText.text = gameName;
        basePriceText.text = basePrice.ToString();
        discountPercentageText.text = discountPercentage.ToString();
        netPriceText.text = netPrice.ToString();
        releaseYearText.text = releaseYear.ToString();
        publisherText.text = publisher;
        ratingText.text = rating.ToString();
        categoryText.text = category;
        buyButton.gameObject.SetActive(true);
        commensButton.gameObject.SetActive(true);
    }

    public void UpdateCategories(string category)
    {
        categories.Add(category);
        string str = "";
        foreach (var VARIABLE in categories)
        {
            str += VARIABLE + "\n";
        }

        categoryText.text = str;
    }

    public void OnBuyButton()
    {
        MainController.instance.gameIndex = index;
        MainController.instance.OpenOrderPanel();
    }
    
    public void OnCommentButton()
    {
        MainController.instance.gameIndex = index;
        MainController.instance.OpenCommentsPanel();
    }
}
