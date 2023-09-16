using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class InteractableObject : MonoBehaviour
{
    [SerializeField] private UIManager uiManager;
    [SerializeField] Vector3 [] scenario1Points;
    private Vector3 defaultPosition;
    private float duration = 3f;
    public int scenarioID = 0;

    private void Start()
    {
        defaultPosition = transform.position;
    }

    private void Scenario1()
    {
        transform.DOKill();
        transform.DOMoveY(transform.position.y + 1.5f, duration / 3f).OnComplete(() => transform.DOMove(new Vector3(transform.position.x + 1.5f, transform.position.y - 1.5f, transform.position.z), duration / 3f).OnComplete(() => transform.DOMoveX(transform.position.x - 1.5f, duration / 3f))).From(defaultPosition);
    }

    private void Scenario2()
    {
        Material material = GetComponent<Renderer>().material;
        material.DOBlendableColor(Color.blue, 1f).OnComplete(() => material.DOBlendableColor(Color.red, 1f).OnComplete(() => material.DOBlendableColor(Color.green,1f).OnComplete(() => material.DOBlendableColor(Color.white, 1f))));
    }

    private void ApplyScenario()
    {
        if(uiManager.scenarioID == 0 && !uiManager.isLocked)
        {
            Scenario1();
        }

        else if(uiManager.scenarioID == 1 && !uiManager.isLocked)
        {
            Scenario2();
        }
    }

    private void OnMouseDown()
    {
        ApplyScenario();
    }

}
