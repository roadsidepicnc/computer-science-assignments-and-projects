struct Bound
{
    glm::vec3 max;
    glm::vec3 min;
};

std::deque <Bound> boundList;

void CreateBounds()
{
    float offset = 0.25f;
    Bound currentBound;
    currentBound.max.x = 20 + offset;
    currentBound.min.x = -20 - offset;
    currentBound.max.z = 21 + offset;
    currentBound.min.z = 19 - offset;
    boundList.push_back(currentBound);

    currentBound.max.x = 20 + offset;
    currentBound.min.x = -20 - offset;
    currentBound.max.z = -19 + offset;
    currentBound.min.z = -21 - offset;
    boundList.push_back(currentBound);

    currentBound.max.z = 20 + offset;
    currentBound.min.z = -20 - offset;
    currentBound.max.x = -19 + offset;
    currentBound.min.x = -21 - offset;
    boundList.push_back(currentBound);

    currentBound.max.z = 20 + offset;
    currentBound.min.z = -20 - offset;
    currentBound.max.x = 21 + offset;
    currentBound.min.x = 19 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = -14 + offset;
    currentBound.min.z = -16 - offset;
    currentBound.max.x = -14 + offset;
    currentBound.min.x = -16 - offset;
    boundList.push_back(currentBound);

    currentBound.max.z = 16 + offset;
    currentBound.min.z = 12 - offset;
    currentBound.max.x = 16 + offset;
    currentBound.min.x = 12 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = 8 + offset;
    currentBound.min.z = 4 - offset;
    currentBound.max.x = 17 + offset;
    currentBound.min.x = 11 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = 15.5 + offset;
    currentBound.min.z = 10.5 - offset;
    currentBound.max.x = -10.5 + offset;
    currentBound.min.x = -15.5 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = 9.5 + offset;
    currentBound.min.z = 7.5 - offset;
    currentBound.max.x = -12 + offset;
    currentBound.min.x = -14 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = -3.25 + offset;
    currentBound.min.z = -4.75 - offset;
    currentBound.max.x = 3 + offset;
    currentBound.min.x = -0.75 - offset;
    boundList.push_back(currentBound);
    
    currentBound.max.z = -11 + offset;
    currentBound.min.z = -21 - offset;
    currentBound.max.x = 21 + offset;
    currentBound.min.x = 13 - offset;
    boundList.push_back(currentBound);
}

bool CheckCollisionHelper(Bound objectBound, glm::vec3 position)
{
    return(objectBound.max.x > position.x && objectBound.min.x < position.x && objectBound.max.z > position.z && objectBound.min.z < position.z);
}

bool CheckCollisionMain(glm::vec3 position)
{
    for (int i = 0; i < boundList.size(); i++)
    {
        if (CheckCollisionHelper(boundList[i], position))
        {
            return true;
        }
    }

    return false;
}