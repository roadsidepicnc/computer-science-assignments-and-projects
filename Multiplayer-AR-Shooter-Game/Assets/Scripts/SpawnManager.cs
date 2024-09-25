using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using Random = UnityEngine.Random;

public class SpawnManager : Singleton<SpawnManager>
{
    [SerializeField] private Enemy enemyPrefab;
    [SerializeField] private Transform spawnPointParent;
    [SerializeField] private List<SpawnPoint> spawnPoints;

    private float spawnTimer;
    private float spawnRate = 1f;
    private int maxTryCount = 1000;

    private void Update()
    {
        if (spawnTimer >= spawnRate)
        {
            TryToSpawnEnemy();
            spawnTimer = 0f;
        }

        spawnTimer += Time.deltaTime;
    }

    private void TryToSpawnEnemy()
    {
        bool isAvailable = IsAnySpawnPointAvailable();
        if (!isAvailable)
        {
            return;
        }
        
        for (int i = 0; i < maxTryCount; i++)
        {
            int index = Random.Range(0, spawnPoints.Count - 1);
            if (!spawnPoints[i].isThereEnemy)
            {
                Enemy enemy = Instantiate(enemyPrefab, spawnPoints[i].transform);
                enemy.spawnPoint = spawnPoints[i];
                spawnPoints[i].isThereEnemy = true;
                break;
            }
        }
    }

    private bool IsAnySpawnPointAvailable()
    {
        bool isAvailable = false;
        for (int i = 0; i < spawnPoints.Count; i++)
        {
            if (!spawnPoints[i].isThereEnemy)
            {
                isAvailable = true;
                break;
            }
        }

        return isAvailable;
    }
}