using UnityEngine;
using System.Collections;

public class Camera : MonoBehaviour {
    public static Transform target;
    public int cameraMode = 1; // motion style
    public float distToCamera = 50f;
	void Start () {
        GameObject obj = GameObject.Find("ShipPlayerGO");
		target = obj.transform;
        transform.position = obj.transform.position;
	}
	
	void Update () 
	{
	    if (target != null ) 
		{
            if (cameraMode == 1)
            {
                Quaternion ship = target.transform.rotation;
                transform.rotation = Quaternion.Lerp(transform.rotation, ship, Time.deltaTime * 3f);
                Vector3 v = new Vector3(-5.0f, 10.0f, -distToCamera);
                Vector3 position1 = transform.rotation * v + target.position;
                transform.position = position1;
            }
            else
            {
                Vector3 position = target.position - this.transform.position;
                this.transform.rotation = Quaternion.LookRotation(position);
                transform.rotation = target.rotation;
                Vector3 v = new Vector3(-5.0f, 10.0f, -distToCamera);
                Vector3 position1 = transform.rotation * v + target.position;
                transform.position = position1;
            }
         
		}
	}
   
}
