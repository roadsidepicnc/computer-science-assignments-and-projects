using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR;
using UnityEngine.XR.ARFoundation;
using TMPro;

public class UIManager : MonoBehaviour
{
    public bool isLocked = true;
    public int scenarioID;
    private int ScenarioCount = 2;
    private ARTrackedImageManager aRTrackedImageManager;
    [SerializeField] private TextMeshProUGUI text;
    [SerializeField] private GameObject lockedText;

    private void Awake()
    {
        aRTrackedImageManager = FindObjectOfType<ARTrackedImageManager>();
    }

    private void Start()
    {
        ChangeText();
    }

    private void ChangeText()
    {
        text.text = "Current Scenario Number is: " + (scenarioID + 1);
    }

    public void NextScenario()
    {    
        if(!isLocked)
        {
            scenarioID = (scenarioID + 1) % ScenarioCount;
            ChangeText();
        }
    }

    public void OnImageChanged(ARTrackedImagesChangedEventArgs args)
    {
        foreach(var image in args.added)
        {
            isLocked = false;
            lockedText.SetActive(false);
        }
    }

    public void OnEnable()
    {
        aRTrackedImageManager.trackedImagesChanged += OnImageChanged;
    }

    public void OnDisable()
    {
        aRTrackedImageManager.trackedImagesChanged -= OnImageChanged;
    }
}
