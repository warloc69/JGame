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
			JavaClient.sendPacket(PacketBuilder.gameObjectFire((short) 2, this.transform.forward));
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

		JavaClient.sendPacket(PacketBuilder.gameObjectMove(this.transform.position, this.transform.rotation));
	}
}
