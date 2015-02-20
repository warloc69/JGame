using UnityEngine;
using System.Collections;

public class Player : MonoBehaviour 
{
	public float speed = 100;
	public float rotationSpeed = 100;
	private CharacterController controller;
	public static bool isActive;

	void Start () 
	{
		controller = GetComponent<CharacterController> ();
		controller.Move (transform.position);
	}
	
	// Update is called once per frame
	void Update () 
	{
		if(!isActive)
			return;

		// fire
		if(Input.GetMouseButtonDown(0))
		{
			Debug.Log("Fire!!! :: 0");
			DynamicObject obj = (DynamicObject) Instantiate(Resources.Load ("Dynamic", typeof(DynamicObject)));
			obj.name = "dynamic";
			obj.transform.position = this.transform.position + this.transform.forward * 1;
			obj.m_velocity = this.transform.forward.normalized * 5;

			JavaClient.sendPacket(PacketBuilder.gameObjectFire(this.GetInstanceID(), (short) 5, obj.m_velocity));
		}

		// rotate by mouse
		if(Input.GetMouseButton(1))
		{
			float h = 2 * Input.GetAxis("Mouse X");
			transform.Rotate(0, h, 0);
		}
		
		float vertical = Input.GetAxis ("Vertical");
		float horizontal = Input.GetAxis ("Horizontal");
		if (vertical == 0 && horizontal == 0) return;
		transform.Rotate (0, rotationSpeed * Time.deltaTime * horizontal, 0);
		controller.Move (transform.forward * speed * Time.deltaTime * vertical);
	}
}
