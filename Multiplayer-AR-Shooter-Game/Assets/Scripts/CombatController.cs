using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CombatController : MonoBehaviour
{
    [SerializeField] private Transform shootingPositionTransform;
    [SerializeField] private LayerMask enemyLayerMask;
    
    private float shootingTimer;
    private float shootingCooldown = 2f;
    
    public bool CanShoot => shootingTimer >= shootingCooldown;

    private void Awake()
    {
        shootingTimer = shootingCooldown;
    }
    
    private void Update()
    {
        shootingTimer += Time.deltaTime;
    }
    
    public void Shoot()
    {
        if (!CanShoot)
        {
            return;
        }
        
        RaycastHit hit;
        if (Physics.Raycast(shootingPositionTransform.position, shootingPositionTransform.TransformDirection(Vector3.forward), out hit, Mathf.Infinity, enemyLayerMask))
        {
            if (hit.collider != null)
            {
                shootingTimer = 0f;
                Enemy enemy = hit.collider.GetComponent<Enemy>();
                enemy.spawnPoint.isThereEnemy = false;
                Destroy(hit.collider.gameObject);
            }
        }
    }
}