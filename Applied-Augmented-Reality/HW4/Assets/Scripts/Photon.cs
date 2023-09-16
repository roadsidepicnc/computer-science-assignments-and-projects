using UnityEngine;

public class Photon : MonoBehaviour {

    [SerializeField] private float coefficient = 1f;
    [SerializeField] private float initialVelocity = 100f;
    
    private GameController gc;
    private int layerMask;
    private Rigidbody rb;
    private Vector3 blackHolecenter;
    private float orbitRadius;
    private Vector3 lastPosition;
    private int x;
    private int y;
    
    private void Awake()
    {
        gc = FindObjectOfType<GameController>();
        rb = GetComponent<Rigidbody>();
        blackHolecenter = gc.blackHole.transform.position;
        orbitRadius = gc.blackHole.transform.localScale.x / 2f;
        layerMask = ~LayerMask.GetMask("Photon");
    }

    public void SetPosition(Vector3 position)
    {
        transform.position = position;
        lastPosition = position;
    }

    public void Initiate(Vector3 velocity, int x, int y)
    {
        rb.velocity = velocity;
        this.x = x;
        this.y = y;
    }
    
    private void Update() {
        //remainingTime += Time.deltaTime / timeToReachTarget;
        //transform.position = Vector3.Slerp(startPos, targetPos, remainingTime);
        //Debug.DrawLine(lastPos, transform.position, Color.red, 1f);
        if (Physics.Raycast(lastPosition, transform.position - lastPosition, out RaycastHit hit, Vector3.Distance(lastPosition, transform.position), layerMask))
        {
            if (hit.transform.CompareTag("BlackHole"))
            {
                gc.SetPixel(x, y, Color.black);
            }
            else 
            {
                Texture2D tex = hit.collider.GetComponent<Renderer>().material.mainTexture as Texture2D;
                gc.SetPixel(x, y, tex.GetPixel((int)(hit.textureCoord.x * tex.width), (int)(hit.textureCoord.y * tex.height)));
            }
            
            rb.velocity = Vector3.zero;
            gc.objectPool.Release(gameObject);
            return;
        }
        if (Vector3.Distance(blackHolecenter, transform.position) < orbitRadius) 
        {
            gc.SetPixel(x, y, Color.black);
            rb.velocity = Vector3.zero;
            gc.objectPool.Release(gameObject);
            return;
        }
        
        rb.AddForce((coefficient / Mathf.Pow(Vector3.Distance(blackHolecenter, transform.position), 2)) * (blackHolecenter - transform.position).normalized, ForceMode.Acceleration);
        lastPosition = transform.position;
    }
}