using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using Unity.Mathematics;

public class MainController : Singleton<MainController>
{
    
    
    [SerializeField] private LibraryItem libraryItemPrefab;
    [SerializeField] private GameListing gameListingPrefab;
    [SerializeField] private OrderItem orderItemPrefab;
    [SerializeField] private CommentItem commentItemPrefab;
    [SerializeField] private NotificationItem notificationItemPrefab;
    [SerializeField] private Transform libraryItemParent;
    [SerializeField] private Transform gameListingParent;
    [SerializeField] private Transform orderItemParent;
    [SerializeField] private Transform commentItemParent;
    [SerializeField] private Transform notificationItemParent;
    [SerializeField] private Transform popUp;
    [SerializeField] private TextMeshProUGUI popUpInfoText;
    
    [SerializeField] private GameObject registerPanel;
    [SerializeField] private GameObject loginPanel;
    [SerializeField] private GameObject customerMainPanel;
    [SerializeField] private GameObject publisherMainPanel;
    [SerializeField] private GameObject publisherCreateGamePanel;
    [SerializeField] private GameObject publisherUpdateGamePanel;
    [SerializeField] private GameObject customerStorePanel;
    [SerializeField] private GameObject customerOrderPanel;
    [SerializeField] private GameObject customerLibraryPanel;
    [SerializeField] private GameObject customerOrdersPanel;
    [SerializeField] private GameObject customerCommentsPanel;
    [SerializeField] private GameObject customerNotificationsPanel;
    
    [SerializeField] private TMP_InputField registerEmailTextField;
    [SerializeField] private TMP_InputField registerNameTextField;
    [SerializeField] private TMP_InputField registerPasswordTextField;
    [SerializeField] private TMP_InputField registerPhoneTextField;
    [SerializeField] private TMP_InputField registerLocationTextField;
    [SerializeField] private TMP_InputField orderAddressTextField;
    [SerializeField] private TMP_InputField publisherGameNameTextField;
    [SerializeField] private TMP_InputField publisherGameBasePriceTextField;
    [SerializeField] private TMP_InputField publisherGameDiscountPercentageTextField;
    [SerializeField] private TMP_InputField publisherGameReleaseYearTextField;
    [SerializeField] private TMP_InputField publisherGameRatingTextField;
    [SerializeField] private TMP_InputField publisherUpdateGameIdTextField;
    [SerializeField] private TMP_InputField publisherUpdateGameBasePriceTextField;
    [SerializeField] private TMP_InputField publisherUpdateGameDiscountTextField;
    [SerializeField] private TMP_InputField customerAddMoneyTextField;
    [SerializeField] private TMP_InputField customerCommentTextField;
    [SerializeField] private TextMeshProUGUI orderInfoText;
    [SerializeField] private TextMeshProUGUI orderMoneyText;
    [SerializeField] private TextMeshProUGUI storeMoneyText;
    
    [SerializeField] private TMP_InputField loginEmailTextField;
    [SerializeField] private TMP_InputField loginPasswordTextField;
    [SerializeField] private TMP_Dropdown registerDropdown;
    [SerializeField] private TMP_Dropdown loginDropdown;
    [SerializeField] private TMP_Dropdown categoryDropdown;
    [SerializeField] private TMP_Dropdown categoryStoreDropdown;
    [SerializeField] private TMP_Dropdown publisherCategoryDropdown;

    private UserType userType;
    private string userEmail;
    public int userId = 1;
    public int gameIndex = 0;
    public int categoryId;
    public int walletMoneyAmount;

    private List<GameListing> gameListings = new List<GameListing>();
    private List<LibraryItem> libraryItems = new List<LibraryItem>();
    private List<Category> categories = new List<Category>();
    private List<OrderItem> orderItems = new List<OrderItem>();
    private List<CommentItem> commentItems = new List<CommentItem>();
    private List<NotificationItem> notificationItems = new List<NotificationItem>();

    public void OpenPanel(string info)
    {
        popUpInfoText.text = info;
        popUp.gameObject.SetActive(true);
    }
    
    public void CallRegister()
    {
        if (registerDropdown.value == 0)
        {
            StartCoroutine(RegisterCustomer());
        }
        
        else if (registerDropdown.value == 1)
        {
            StartCoroutine(RegisterPublisher());
        }
    }
    
    public void CallLogin()
    {
        if (loginDropdown.value == 0)
        {
            StartCoroutine(LoginCustomer());
        }
        
        else
        {
            StartCoroutine(LoginPublisher());
        }
    }
    
    public void CallViewAllGames()
    {
        StartCoroutine(ViewAllGames());
    }
    
    public void CallSortGamesByPrice()
    {
        StartCoroutine(SortGamesByPrice());
    }
    
    public void CallSortGamesByName()
    {
        StartCoroutine(SortGamesByName());
    }
    
    public void CallSortGamesByRating()
    {
        StartCoroutine(SortGamesByRating());
    }
    
    public void CallCreateOrder()
    {
        StartCoroutine(CreateOrder());
    }
    
    public void CallGetWalletMoneyAmount()
    {
        StartCoroutine(GetWalletMoneyAmount());
    }
    
    public void CallViewLibrary()
    {
        StartCoroutine(ViewLibrary());
        OpenLibraryPanel();
    }
    
    public void CallFilterGamesByCategory()
    {
        StartCoroutine(FilterGamesByCategory());
    }
    
    public void CallViewAllCategories()
    {
        StartCoroutine(ViewAllCategories());
    }
    
    public void CallViewAllOrders()
    {
        StartCoroutine(ViewAllOrders());
        OpenLibraryPanel();
    }

    public void CallDeleteCustomer()
    {
        StartCoroutine(DeleteCustomer());
    }
    
    public void CallAddMoneyToWallet()
    {
        StartCoroutine(AddMoneyToWallet());
    }
    
    public void CallCreateGame()
    {
        StartCoroutine(CreateGame());
    }
    
    public void CallUpdateGame()
    {
        StartCoroutine(UpdateGame());
    }
    
    public void CallCreateComment()
    {
        StartCoroutine(CreateComment());
    }
    
    public void CallViewComments()
    {
        StartCoroutine(ViewComments());
    }
    
    public void CallViewNotifications()
    {
        StartCoroutine(ViewNotifications());
    }
    
    private IEnumerator RegisterCustomer()
    {
        
        string name = registerNameTextField.text;
        string password = registerPasswordTextField.text;
        string email = registerEmailTextField.text;
        string phone = registerPhoneTextField.text;

        if (name == "" || password == "" || email == "" || phone == "")
        {
            string popUpInfo = "Customer register failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);  
            yield break;
        }

        WWWForm form = new WWWForm();
        form.AddField("name", name);
        form.AddField("password", password);
        form.AddField("email", email);
        form.AddField("phone", Int32.Parse(phone));
        WWW www = new WWW("http://localhost/sqlconnect/registercustomer.php", form);
        yield return www;
        
        
        if (www.text != "0")
        {
            userId = Int32.Parse(www.text);
            string popUpInfo = "Customer registered and logged in successfully. Customer's id is " + userId;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            userType = UserType.Customer;
            OpenCustomerMainPanel();
        }
        else
        {
            string popUpInfo = "Customer register failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);        
        }
    }
    
    private IEnumerator RegisterPublisher()
    {
        string name = registerNameTextField.text;
        string password = registerPasswordTextField.text;
        string email = registerEmailTextField.text;
        string phone = registerPhoneTextField.text;
        string location = registerLocationTextField.text;

        if (name == "" || password == "" || email == "" || phone == "" || location == "")
        {
            string popUpInfo = "Publisher register failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);  
            yield break;
        }

        WWWForm form = new WWWForm();
        form.AddField("name", name);
        form.AddField("password", password);
        form.AddField("email", email);
        form.AddField("phone", Int32.Parse(phone));
        form.AddField("location", location);
        form.AddField("phone", Int32.Parse(phone));
        WWW www = new WWW("http://localhost/sqlconnect/registerpublisher.php", form);
        yield return www;

        if (www.text != "0")
        {
            userId = Int32.Parse(www.text);
            string popUpInfo = "Publisher registered and logged in successfully. Publisher's id is " + userId;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            userType = UserType.Publisher;
            OpenPublisherMainPanel();
        }
        else
        {
            string popUpInfo = "Publisher register failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);
        }
    }
    
    private IEnumerator LoginCustomer()
    {
        WWWForm form = new WWWForm();
        form.AddField("email", loginEmailTextField.text);
        form.AddField("password", loginPasswordTextField.text);
        WWW www = new WWW("http://localhost/sqlconnect/logincustomer.php", form);
        yield return www;
        string[] items1 = www.text.Split("|");
        
        if (items1[0] == "SUCCESSFUL")
        {
            string[] items2 = www.text.Split("'");
            userId = Int32.Parse(items2[1]);
            string popUpInfo = "Customer logged in successfully. Customer's id is " + userId;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            OpenCustomerMainPanel();
            userType = UserType.Customer;
        }
        else
        {
            string popUpInfo = "Customer login failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);
        }
    }
    
    private IEnumerator LoginPublisher()
    {
        WWWForm form = new WWWForm();
        form.AddField("email", loginEmailTextField.text);
        form.AddField("password", loginPasswordTextField.text);
        WWW www = new WWW("http://localhost/sqlconnect/loginpublisher.php", form);
        yield return www;
        string[] items1 = www.text.Split("|");
        
        if (items1[0] == "SUCCESSFUL")
        {
            string[] items2 = www.text.Split("'");
            userId = Int32.Parse(items2[1]);
            string popUpInfo = "Publisher logged in successfully. Publisher's id is " + userId;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            userType = UserType.Publisher;
            OpenPublisherMainPanel();
        }
        else
        {
            string popUpInfo = "Publisher login failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);
        }
    }
    
    private IEnumerator ViewAllGames()
    {
        WWW www = new WWW("http://localhost/sqlconnect/viewallgames.php");
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in gameListings)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            gameListings.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = gameListings.Find(x => x.id == Int32.Parse(items2[0]));
                if (temp != null)
                {
                    temp.UpdateCategories(items2[8]);
                    continue;    
                }

                GameListing gl = Instantiate(gameListingPrefab, gameListingParent);
                gl.SetTexts(Int32.Parse(items2[0]), items2[1], Int32.Parse(items2[2]), Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]),  items2[6], Int32.Parse(items2[7]), items2[8]);
                gl.index = counter;
                gameListings.Add(gl);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator SortGamesByPrice()
    {
        WWW www = new WWW("http://localhost/sqlconnect/sortgamesbyprice.php");
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in gameListings)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            gameListings.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = gameListings.Find(x => x.id == Int32.Parse(items2[0]));
                if (temp != null)
                {
                    temp.UpdateCategories(items2[8]);
                    continue;    
                }

                GameListing gl = Instantiate(gameListingPrefab, gameListingParent);
                gl.SetTexts(Int32.Parse(items2[0]), items2[1], Int32.Parse(items2[2]), Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]),  items2[6], Int32.Parse(items2[7]), items2[8]);
                gl.index = counter;
                gameListings.Add(gl);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator SortGamesByName()
    {
        WWW www = new WWW("http://localhost/sqlconnect/sortgamesbyname.php");
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in gameListings)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            gameListings.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = gameListings.Find(x => x.id == Int32.Parse(items2[0]));
                if (temp != null)
                {
                    temp.UpdateCategories(items2[8]);
                    continue;    
                }

                GameListing gl = Instantiate(gameListingPrefab, gameListingParent);
                gl.SetTexts(Int32.Parse(items2[0]), items2[1], Int32.Parse(items2[2]), Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]),  items2[6], Int32.Parse(items2[7]), items2[8]);
                gl.index = counter;
                gameListings.Add(gl);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator SortGamesByRating()
    {
        WWW www = new WWW("http://localhost/sqlconnect/sortgamesbyrating.php");
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in gameListings)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            gameListings.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = gameListings.Find(x => x.id == Int32.Parse(items2[0]));
                if (temp != null)
                {
                    temp.UpdateCategories(items2[8]);
                    continue;    
                }

                GameListing gl = Instantiate(gameListingPrefab, gameListingParent);
                gl.SetTexts(Int32.Parse(items2[0]), items2[1], Int32.Parse(items2[2]), Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]),  items2[6], Int32.Parse(items2[7]), items2[8]);
                gl.index = counter;
                gameListings.Add(gl);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator CreateOrder()
    {
        if (orderAddressTextField.text == "")
        {
            string info = "Order failed";
            OpenPanel(info);
            Debug.Log(info);
            yield break;
        }
        
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        form.AddField("gameid", gameListings[gameIndex].id);
        form.AddField("address", orderAddressTextField.text);
        form.AddField("gameprice", gameListings[gameIndex].netPrice);
        WWW www = new WWW("http://localhost/sqlconnect/createorder.php", form);
        yield return www;

        
        if (www.text != "0")
        {
            string info = "Order created successfully. Order id is " + www.text;
            OpenPanel(info);
            Debug.Log(info);
            customerMainPanel.SetActive(true);
            customerOrderPanel.SetActive(false);
        }
        else
        {
            string info = "Order failed";
            OpenPanel(info);
            Debug.Log(info);
        }
    }
    
    private IEnumerator GetWalletMoneyAmount()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        WWW www = new WWW("http://localhost/sqlconnect/viewwalletmoneyamount.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            walletMoneyAmount = Int32.Parse(items1[1]);
            Debug.Log("View wallet money amount query is successful: " + items1[1]);
            orderMoneyText.text = "Wallet Balance: " + walletMoneyAmount;
            storeMoneyText.text = "Wallet Balance: " + walletMoneyAmount;
        }
        else
        {
            Debug.Log("View wallet money amount query is unsuccessful. Error #" + backup);
        }
    }
    
    private IEnumerator ViewLibrary()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        WWW www = new WWW("http://localhost/sqlconnect/viewlibrary.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in libraryItems)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            libraryItems.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = libraryItems.Find(x => x.gameName == items2[0]);
                if (temp != null)
                {
                    temp.UpdateCategories(items2[2]);
                    continue;    
                }

                LibraryItem li = Instantiate(libraryItemPrefab, libraryItemParent);
                li.SetTexts(items2[0], items2[1], items2[2], Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]));
                li.index = counter;
                libraryItems.Add(li);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator ViewComments()
    {
        foreach (var VARIABLE in commentItems)
        {
            Destroy(VARIABLE.gameObject);
        }
            
        commentItems.Clear();
        
        WWWForm form = new WWWForm();
        int gameId = gameListings[gameIndex].id;
        form.AddField("gameid", gameId);
        WWW www = new WWW("http://localhost/sqlconnect/viewcomments.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");

        Debug.Log("XX: " + gameId);

        if (items1[0] == "SUCCESSFUL")
        {
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                CommentItem ci = Instantiate(commentItemPrefab, commentItemParent);
                ci.SetTexts(items2[0], items2[1]);
                ci.index = counter;
                commentItems.Add(ci);
                counter++;

            }
            
            
            Debug.Log("View all comments query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("Comment query in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator FilterGamesByCategory()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        form.AddField("categoryid", categoryId);
        WWW www = new WWW("http://localhost/sqlconnect/filtergamesbycategory.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in libraryItems)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            libraryItems.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                var temp = libraryItems.Find(x => x.gameName == items2[0]);
                if (temp != null)
                {
                    temp.UpdateCategories(items2[2]);
                    continue;    
                }

                LibraryItem li = Instantiate(libraryItemPrefab, libraryItemParent);
                li.SetTexts(items2[0], items2[1], items2[2], Int32.Parse(items2[3]), Int32.Parse(items2[4]), Int32.Parse(items2[5]));
                li.index = counter;
                libraryItems.Add(li);
                counter++;
            }
            
            
            Debug.Log("View all games query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator ViewAllCategories()
    {
        WWW www = new WWW("http://localhost/sqlconnect/viewallcategories.php");
        yield return www;
        string backup = String.Copy(www.text);
        Debug.Log(www.text);

        string[] items1 = www.text.Split("\n");

        if (items1[0] == "SUCCESSFUL")
        {
            categories.Clear();
            int counter = 0;

            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                Category c = new Category();
                c.id = Int32.Parse(items2[0]);
                c.name = items2[2];
                c.rating = Int32.Parse(items2[1]);
                categories.Add(c);
            }

            counter++;
            LoadCategoryDropdown();
            Debug.Log("View all games categories is successful");
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + backup);
        }
    }
    
    private IEnumerator ViewAllOrders()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        WWW www = new WWW("http://localhost/sqlconnect/viewallorders.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in orderItems)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            orderItems.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                OrderItem oi = Instantiate(orderItemPrefab, orderItemParent);
                oi.SetTexts(counter, Int32.Parse(items2[0]), items2[1], items2[2], Int32.Parse(items2[3]), items2[4], items2[5], Int32.Parse(items2[6]), items2[7], items2[8]);
                oi.index = counter;
                orderItems.Add(oi);
                counter++;
            }
            
            Debug.Log("View all orders query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator ViewNotifications()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        WWW www = new WWW("http://localhost/sqlconnect/viewnotifications.php", form);
        yield return www;
        string backup = String.Copy(www.text);
        string[] items1 = www.text.Split("\n");
        
        if (items1[0] == "SUCCESSFUL")
        {
            foreach (var VARIABLE in notificationItems)
            {
                Destroy(VARIABLE.gameObject);
            }
            
            notificationItems.Clear();
            int counter = 0;
            for (int i = 1; i < items1.Length - 1; i++)
            {
                string[] items2 = items1[i].Split("|");
                NotificationItem ni = Instantiate(notificationItemPrefab, notificationItemParent);
                ni.SetTexts(items2[0], items2[1]);
                ni.index = counter;
                notificationItems.Add(ni);
                counter++;
            }
            
            Debug.Log("View all orders query is successful");
            Debug.Log(backup);
        }
        else
        {
            Debug.Log("User logged in unsuccessfully. Error #" + www.text);
        }
    }
    
    private IEnumerator DeleteCustomer()
    {
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        WWW www = new WWW("http://localhost/sqlconnect/deletecustomer.php", form);
        yield return www;
        string info = "Customer is deleted. Customer ID is " + userId;
        OpenPanel(info);
        Debug.Log(info);
        loginEmailTextField.text = "";
        loginPasswordTextField.text = "";
        registerEmailTextField.text = "";
        registerLocationTextField.text = "";
        registerNameTextField.text = "";
        registerPasswordTextField.text = "";
        registerPhoneTextField.text = "";
        loginPanel.SetActive(true);
        customerMainPanel.SetActive(false);
        publisherMainPanel.SetActive(false);
        customerLibraryPanel.SetActive(false);
        userId = 0;
    }
    
    private IEnumerator AddMoneyToWallet()
    {
        string amount = customerAddMoneyTextField.text;
        if (amount == "")
        {
            string popUpInfo = "Add money failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);  
            yield break;
        }
        
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        form.AddField("moneyamount", Int32.Parse(amount));
        WWW www = new WWW("http://localhost/sqlconnect/addmoney.php", form);
        yield return www;
        string info = "Money added to wallet. Customer ID is " + userId + ". Amount is " + amount;
        OpenPanel(info);
        Debug.Log(info);
        CallGetWalletMoneyAmount();
    }
    
    private IEnumerator CreateGame()
    {
        string name = publisherGameNameTextField.text;
        string price = publisherGameBasePriceTextField.text;
        string discount = publisherGameDiscountPercentageTextField.text;
        string releaseYear = publisherGameReleaseYearTextField.text;
        string rating = publisherGameRatingTextField.text;
        int temp = categoryDropdown.value;
        int cid = categories[temp].id;
        
        if (name == "" || price == "" || discount == "" || releaseYear == "" || rating == "")
        {
            string popUpInfo = "Create game failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);  
            yield break;
        }

        WWWForm form = new WWWForm();
        form.AddField("userid", userId);
        form.AddField("gamename", name);
        form.AddField("baseprice", Int32.Parse(price));
        form.AddField("discount", Int32.Parse(discount));
        form.AddField("releaseyear", Int32.Parse(releaseYear));
        form.AddField("rating", Int32.Parse(rating));
        form.AddField("categoryid", cid);
        WWW www = new WWW("http://localhost/sqlconnect/creategame.php", form);
        yield return www;

        if (www.text != "0")
        {
            string popUpInfo = name + " game is created. Games's id is " + www.text;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            OpenPublisherMainPanel();
        }
        else
        {
            string popUpInfo = "Create game failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo); 
        }
    }
    
    private IEnumerator UpdateGame()
    {
        string gameId = publisherUpdateGameIdTextField.text;
        string price = publisherUpdateGameBasePriceTextField.text;
        string discount = publisherUpdateGameDiscountTextField.text;
        

        if (gameId == "" || price == "" || discount == "")
        {
            string popUpInfo = "Update game failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo);  
            yield break;
        }

        WWWForm form = new WWWForm();
        form.AddField("userid", userId);
        form.AddField("gameid", gameId);
        form.AddField("baseprice", Int32.Parse(price));
        form.AddField("discount", Int32.Parse(discount));
        WWW www = new WWW("http://localhost/sqlconnect/updategame.php", form);
        yield return www;

        if (www.text != "0")
        {
            string popUpInfo = "Game is updated. Games's id is " + www.text;
            Debug.Log(popUpInfo);
            OpenPanel(popUpInfo);
            OpenPublisherMainPanel();
        }
        else
        {
            string popUpInfo = "Update game failed";
            OpenPanel(popUpInfo);
            Debug.Log(popUpInfo); 
        }
    }
    
    private IEnumerator CreateComment()
    {
        if (customerCommentTextField.text == "")
        {
            string info = "Comment failed";
            OpenPanel(info);
            Debug.Log(info);
            yield break;
        }
        
        WWWForm form = new WWWForm();
        form.AddField("customerid", userId);
        form.AddField("gameid", libraryItems[gameIndex].id);
        form.AddField("content", customerCommentTextField.text);
  
        WWW www = new WWW("http://localhost/sqlconnect/createcomment.php", form);
        yield return www;
        
        if (www.text != "0")
        {
            string info = "Comment created successfully.";
            OpenPanel(info);
            Debug.Log(info);
        }
        else
        {
            string info = "Comment failed";
            OpenPanel(info);
            Debug.Log(www.text);
        }
    }

    public void OnRegisterDropdownValueChanged()
    {
        if (registerDropdown.value == 0)
        {
            registerLocationTextField.gameObject.SetActive(false);
        }
        else
        {
            registerLocationTextField.gameObject.SetActive(true);
        }
    }
    
    public void OnCategoryDropdownValueChanged()
    {
        categoryId = categories[categoryDropdown.value].id;
    }
    
    public void OnCategoryDropdownValueChangedPublisher()
    {
        categoryId = categories[publisherCategoryDropdown.value].id;
    }
    
    public void OnCategoryDropdownValueChangedlibrary()
    {
        categoryId = categories[categoryStoreDropdown.value].id;
    }
    
    public void OpenStorePanel()
    {
        CallViewAllGames();
        CallGetWalletMoneyAmount();
        customerStorePanel.SetActive(true);
        customerMainPanel.SetActive(false);
    }

    public void OpenOrderPanel()
    {
        CallGetWalletMoneyAmount();
        customerStorePanel.SetActive(false);
        customerOrderPanel.SetActive(true);
        orderInfoText.text = "";
        foreach (var VARIABLE in gameListings[gameIndex].categories)
        {
            orderInfoText.text += VARIABLE + " - ";
        }
        
        orderInfoText.text += gameListings[gameIndex].publisher + " - " + gameListings[gameIndex].gameName + " - " + gameListings[gameIndex].netPrice;
    }

    public void OpenNotificationsPanel()
    {
        CallViewNotifications();
        customerNotificationsPanel.SetActive(true);
        customerMainPanel.SetActive(false);
    }

    public void OpenLibraryPanel()
    {
        CallViewAllCategories();
        customerMainPanel.SetActive(false);
        customerLibraryPanel.SetActive(true);
    }
    
    public void OpenCommentsPanel()
    {
        CallViewComments();
        customerStorePanel.SetActive(false);
        customerCommentsPanel.SetActive(true);
    }
    
    public void OpenOrdersPanel()
    {
        CallViewAllOrders();
        customerLibraryPanel.SetActive(false);
        customerOrdersPanel.SetActive(true);
    }

    public void LoadCategoryDropdown()
    {
        categoryDropdown.ClearOptions();
        categoryStoreDropdown.ClearOptions();
        List<string> m_DropOptions = new List<string>();
        foreach (var VARIABLE in categories)
        {
            m_DropOptions.Add(VARIABLE.name);
        }
        
        categoryDropdown.AddOptions(m_DropOptions);
        categoryStoreDropdown.AddOptions(m_DropOptions);
    }

    public void OpenPublisherMainPanel()
    {
        CallViewAllCategories();
        publisherMainPanel.SetActive(true);     
        publisherCreateGamePanel.SetActive(false);     
        loginPanel.SetActive(false);     
        registerPanel.SetActive(false);
        
    }
    
    public void OpenCustomerMainPanel()
    {
        customerMainPanel.SetActive(true);     
        loginPanel.SetActive(false);     
        registerPanel.SetActive(false);
    }

    public void LogOut()
    {
        loginEmailTextField.text = "";
        loginPasswordTextField.text = "";
        registerEmailTextField.text = "";
        registerLocationTextField.text = "";
        registerNameTextField.text = "";
        registerPasswordTextField.text = "";
        registerPhoneTextField.text = "";
        string info = "User logged out successfully. User id is " + userId;
        OpenPanel(info);
        Debug.Log(info);
        loginPanel.SetActive(true);
        customerMainPanel.SetActive(false);
        publisherMainPanel.SetActive(false);
        userId = 0;
    }
}

public enum UserType
{
    Customer,
    Publisher
}

public struct Category
{
    public int id;
    public string name;
    public int rating;
};