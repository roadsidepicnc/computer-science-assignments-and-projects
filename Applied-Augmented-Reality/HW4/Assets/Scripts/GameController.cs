using System.Collections;
using UnityEngine;
using UnityEngine.Pool;
using UnityEngine.UI;

public class GameController : MonoBehaviour
{
    public GameObject blackHole;
    public ObjectPool<GameObject> objectPool;
    
    private int layerMask;
    private int objectPoolSize = 1000;
    private Texture2D imageTexture;
    private Camera camera;

    [SerializeField] private RawImage targetImage;
    [SerializeField] private GameObject photonPrefab;
    [SerializeField] private Transform objectPoolParent;
    
    private void GetFromPool(GameObject go) 
    {
        go.transform.position = camera.transform.position;
        go.SetActive(true);
    }
    
    void Start()
    {
        layerMask = ~LayerMask.GetMask("Photon");
        objectPool = new ObjectPool<GameObject>(() => Instantiate(photonPrefab, camera.transform.position, Quaternion.identity, objectPoolParent), GetFromPool, (x) => x.SetActive(false), null, true, objectPoolSize, objectPoolSize);
        camera = Camera.main;
        SetBlankImage();
        StartCoroutine("RayCastAndPaint");
    }
    
    private IEnumerator RayCastAndPaint() 
    {
        RaycastHit hit;
        for (int i = 0; i < Screen.width; i++) 
        {
            for (int j = 0; j < Screen.height; j++) 
            {
                Vector3 direction = camera.ScreenToWorldPoint(new Vector3(i, j, camera.farClipPlane)) - camera.transform.position;
                if (Physics.Raycast(camera.transform.position, direction, out hit, camera.farClipPlane, layerMask))
                {
                    if (hit.transform.CompareTag("BlackHole"))
                    {
                        while (objectPool.CountActive > objectPoolSize - 1)
                        {
                            yield return null;
                        }
                        
                        GameObject obj = objectPool.Get();
                        Photon photon = obj.GetComponent<Photon>();
                        photon.Initiate(direction.normalized * 50, i, j);
                        photon.GetComponent<Photon>().SetPosition(hit.point);
                    }
                    else 
                    {
                        Texture2D tex = hit.collider.GetComponent<Renderer>().material.mainTexture as Texture2D;
                        imageTexture.SetPixel(i, j, tex.GetPixel((int)(hit.textureCoord.x * tex.width), (int)(hit.textureCoord.y * tex.height)));
                    }
                }
            }
        }
        
        yield return new WaitForSeconds(2f);
        Debug.Log("Done Executing!");
        imageTexture.Apply();
    }

    public void SetPixel(int x, int y, Color color) 
    {
        imageTexture.SetPixel(x, y, color);
    }

    private void SetBlankImage() 
    {
        imageTexture = new Texture2D(640, 480);
        for (int i = 0; i < imageTexture.width; i++)
        {
            for (int j = 0; j < imageTexture.height; j++)
            {
                imageTexture.SetPixel(i, j, Color.clear);

            }
        }

        imageTexture.Apply();
        targetImage.texture = imageTexture;
    }
}
