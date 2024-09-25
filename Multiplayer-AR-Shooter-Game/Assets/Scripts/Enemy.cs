using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    private float shootingRate;
    private float damageAmount;
    private float shootingTimer;

    public SpawnPoint spawnPoint;
    
    private void Update()
    {
        if (shootingTimer >= shootingRate)
        {
            TryToShoot();
        }

        shootingRate += Time.deltaTime;
    }

    private void TryToShoot()
    {
        
    }
    
    private void Shoot()
    {
        shootingTimer = 0f;
    }
}