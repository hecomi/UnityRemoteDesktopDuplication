using UnityEngine;

namespace uNvPipe.Examples
{

public class Rotator : MonoBehaviour
{
    [SerializeField]
    Vector3 angleSpeed = new Vector3(0, 180, 0);

    void Update()
    {
        transform.localEulerAngles += angleSpeed * Time.deltaTime;
    }
}

}
