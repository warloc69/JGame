using UnityEngine;
using System.Collections;

public class Camera : MonoBehaviour {
//	public Transform target ;
    public Rigidbody targetRB  ;
    float distance = 10.0f;
float height = 5.0f;
float heightDamping = 2.0f;
float rotationDamping = 3.0f;
public static Transform target;

	// Use this for initialization
	void Start () {
     //   GameObject obj = (GameObject)Instantiate(Resources.Load("ShipPlayerGO", typeof(GameObject)));
        GameObject obj = GameObject.Find("ShipPlayerGO");
		Camera.target = obj.transform;

	}
	
	// Update is called once per frame
	void Update () 
	{
	    if (target != null) 
		{
			// move camera behind the object
		//	Vector3 pos = target.position;
		//	pos.y += upDist;
		//	Vector3 behind = target.forward * behindDist;
		//	behind = Vector3.ClampMagnitude(behind, behindDist);
		//	transform.position = pos - behind;
            Vector3 position =  target.position - this.transform.position;
            this.transform.rotation = Quaternion.LookRotation(position);
			transform.rotation = target.rotation;
            Vector3 v = new Vector3(-5.0f, 10.0f,  - 20f);
            Vector3 position1 = transform.rotation * v + target.position;
            transform.position = position1;

			// check camera collisions
		/*	Vector3 relativePos = transform.position - target.position;
			RaycastHit hit;
			if (Physics.Raycast(target.position, relativePos, out hit, behindDist + 0.5f)) 
			{
				distanceOffset = behindDist - hit.distance + 0.5f;
				distanceOffset = Mathf.Clamp(distanceOffset, 0, behindDist);
			} else {
				distanceOffset = 0;
			}
			transform.position += target.forward * distanceOffset;
         */
		}
	}
   
}
