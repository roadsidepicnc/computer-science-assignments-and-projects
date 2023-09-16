using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement : MonoBehaviour
{
    private float movementSpeed = 2.5f;
    private float lastDirectionTime = 0f;
    private int direction = 1;
    private float directionDuration = 1.35f;

    private void Update()
    {
        if(Time.time >= lastDirectionTime + directionDuration)
        {
            lastDirectionTime = Time.time;
            direction *= -1;
        }

        transform.Translate(Vector3.up * movementSpeed * direction * Time.deltaTime);  
    }

}
