using UnityEngine;
using System.Collections;

public class Camera : MonoBehaviour {
	public static Transform target;
	public float behindDist = 10;
	public float upDist = 5;
	private float distanceOffset = 0;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () 
	{
	    if (target != null) 
		{
			// move camera behind the object
			Vector3 pos = target.position;
			pos.y += upDist;
			Vector3 behind = target.forward * behindDist;
			behind = Vector3.ClampMagnitude(behind, behindDist);
			transform.position = pos - behind;
			transform.rotation = target.rotation;

			// check camera collisions
			Vector3 relativePos = transform.position - target.position;
			RaycastHit hit;
			if (Physics.Raycast(target.position, relativePos, out hit, behindDist + 0.5f)) 
			{
				distanceOffset = behindDist - hit.distance + 0.5f;
				distanceOffset = Mathf.Clamp(distanceOffset, 0, behindDist);
			} else {
				distanceOffset = 0;
			}
			transform.position += target.forward * distanceOffset;
		}
	}
}
