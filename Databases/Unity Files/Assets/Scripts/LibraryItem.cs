using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class LibraryItem : MonoBehaviour
{
    public TextMeshProUGUI nameText;    
    public TextMeshProUGUI releaseYearText;    
    public TextMeshProUGUI publisherText;    
    public TextMeshProUGUI ratingText;
    public TextMeshProUGUI categoryText;
    
    public Button commentButton;

    public int index;
    public int id;
    public string gameName;
    public int releaseYear;
    public string publisher;
    public int rating;
    public List<string> categories;

    public void SetTexts(string gameName, string publisher, string category, int releaseYear, int rating, int id)
    {
        this.gameName = gameName;
        this.releaseYear = releaseYear;
        this.publisher = publisher;
        this.rating = rating;
        this.id = id;
        categories.Add(category);

        nameText.text = gameName;
        releaseYearText.text = releaseYear.ToString();
        publisherText.text = publisher;
        ratingText.text = rating.ToString();
        categoryText.text = category;
        commentButton.gameObject.SetActive(true);
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

    public void OnCommentButton()
    {
        MainController.instance.gameIndex = index;
        MainController.instance.CallCreateComment();
    }
}
