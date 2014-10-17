/**
* \file PrtProgram.h
* \brief Defines the representation of P programs in C.
* A P program will be compiled into a set of constant expressions
* using these data structures.
*/
#ifndef PRTPROGRAM_H
#define PRTPROGRAM_H

#ifdef __cplusplus
extern "C"{
#endif

#include "PrtValues.h"

/** A PRT_SM_FUN function is a pointer to a P function.
*   context is the current machine context.
*   machIndex is the machine decl index where this function is defined.
*   funIndex is the function decl index of this function.
*   value is the argument to this function. It will be the P null value if this function is nullary.
*   Function frees value.
*/
typedef void(PRT_CALL_CONV *PRT_SM_FUN)(
	_Inout_ struct PRT_SMCONTEXT * context, 
	_In_    PRT_UINT32 machIndex, 
	_In_    PRT_UINT32 funIndex, 
	_Inout_ PRT_VALUE *value);

/** A PRT_SM_EXTCTOR function constructs the external blob attached to a machine.
*   context is the machine context to construct.
*   value is the value passed to the new M(...) operation. It will be the P null value if no value was passed.
*   Function frees value.
*/
typedef void(PRT_CALL_CONV *PRT_SM_EXTCTOR)(_Inout_ struct PRT_SMCONTEXT * context, _Inout_ PRT_VALUE *value);

/** A PRT_SM_EXTDTOR function destructs the external blob attached to a machine.
*   context is the machine context to destruct.
*/
typedef void(PRT_CALL_CONV *PRT_SM_EXTDTOR)(_Inout_ struct PRT_SMCONTEXT * context);

/** A PRT_SM_MODELNEW function constructs a model machine in process.
*   Returns a new model identifier.
*   value is the value passed to the new M(...) operation. It will be the P null value if no value was passed.
*   Function frees value.
*/
typedef PRT_VALUE *(PRT_CALL_CONV *PRT_SM_MODELNEW)(_In_ struct PRT_PROCESS * process, _Inout_ PRT_VALUE *value);

/** A PRT_SM_MODELSEND function sends an event to a model machine.
*  process is the calling process.
*  id is the model id of the target machine.
*  event is the id of the event being sent.
*  payload is the data being sent.
*  Function frees id, event, payload.
*/
typedef void(PRT_CALL_CONV *PRT_SM_MODELSEND)(
	_In_ struct PRT_PROCESS * process, 
	_Inout_ PRT_VALUE *id, 
	_Inout_ PRT_VALUE *evnt, 
	_Inout_ PRT_VALUE *payload);

/** A PRT_SM_MODELSHUTDOWN function is called when a model's process is shutdown.
*  process is the process shutting down.
*  modelid is the model id of the model to be shut down.
*   Function frees value.
*/
typedef PRT_VALUE *(PRT_CALL_CONV *PRT_SM_MODELSHUTDOWN)(_In_ struct PRT_PROCESS * process, _Inout_ PRT_VALUE *modelid);

/** Represents a P event declaration */
typedef struct PRT_EVENTDECL
{
	PRT_UINT32 declIndex;         /**< The index of event in program                                           */
	PRT_STRING name;              /**< The name of this event set                                              */
	PRT_UINT32 eventMaxInstances; /**< The value of maximum instances of the event that can occur in the queue */
	PRT_TYPE   *type;	          /**< The type of the payload associated with this event                      */

	PRT_UINT32 nAnnotations;      /**< Number of annotations                                                   */
	void       **annotations;     /**< An array of annotations                                                 */
} PRT_EVENTDECL;

/** Represents a set of P events and the set packed into a bit vector */
typedef struct PRT_EVENTSETDECL
{
	PRT_UINT32 declIndex;      /**< The index of event set in the program  */
	PRT_UINT32 *packedEvents;  /**< The events packed into an array of ints */
} PRT_EVENTSETDECL;

/** Represents a P variable declaration */
typedef struct PRT_VARDECL
{
	PRT_UINT32 declIndex;      /**< The index of variable in owner machine */
	PRT_UINT32 ownerMachIndex; /**< The index of owner machine in program  */
	PRT_STRING name;           /**< The name of this variable              */
	PRT_TYPE   *type;          /**< The type of this variable              */

	PRT_UINT32 nAnnotations;   /**< Number of annotations                  */
	void       **annotations;  /**< An array of annotations                */
} PRT_VARDECL;

/** Represents a P function declaration */
typedef struct PRT_FUNDECL
{
	PRT_UINT32 declIndex;      /**< The index of variable in owner machine        */
	PRT_UINT32 ownerMachIndex; /**< The index of owner machine in program         */
	PRT_STRING name;           /**< The name of this function (NULL is anonymous) */
	PRT_SM_FUN implementation; /**< The implementation of this function           */

	PRT_UINT32 nAnnotations;   /**< Number of annotations                         */
	void       **annotations;  /**< An array of annotations                       */
} PRT_FUNDECL;

/** Represents a P transition declaration */
typedef struct PRT_TRANSDECL
{
	PRT_UINT32  declIndex;         /**< The index of this decl in owner state           */
	PRT_UINT32  ownerStateIndex;   /**< The index of owner state in owner machine       */
	PRT_UINT32  ownerMachIndex;    /**< The index of owner machine in program           */
	PRT_UINT32  triggerEventIndex; /**< The index of the trigger event in program       */
	PRT_UINT32  destStateIndex;    /**< The index of destination state in owner machine */
	PRT_UINT32  transFunIndex;     /**< The index of function to execute when this transition is triggered */
	PRT_BOOLEAN isPush;            /**< True if owner state is pushed onto state stack  */

	PRT_UINT32  nAnnotations;      /**< Number of annotations                         */
	void        **annotations;     /**< An array of annotations                       */
} PRT_TRANSDECL;

/** Represents a P do declaration */
typedef struct PRT_DODECL
{
	PRT_UINT32      declIndex;         /**< The index of this decl in owner state                  */
	PRT_UINT32      ownerStateIndex;   /**< The index of owner state in owner machine              */
	PRT_UINT32      ownerMachIndex;    /**< The index of owner machine in program                  */
	PRT_STRING      name;              /**< The name of this action                                */
	PRT_UINT32      triggerEventIndex; /**< The index of the trigger event in program              */
	PRT_UINT32      doFunIndex;        /**< The index of function to execute when this do is triggered  */

	PRT_UINT32      nAnnotations;      /**< Number of annotations                         */
	void            **annotations;     /**< An array of annotations                       */
} PRT_DODECL;

/** Represents a P state declaration */
typedef struct PRT_STATEDECL
{
	PRT_UINT32  declIndex;       /**< The index of state in owner machine    */
	PRT_UINT32  ownerMachIndex;  /**< The index of owner machine in program  */
	PRT_STRING  name;            /**< The name of this state                 */
	PRT_UINT32  nTransitions;    /**< The number of transitions              */
	PRT_UINT32  nDos;            /**< The number of do handlers              */

	PRT_UINT32      defersSetIndex; /**< The index of the defers set in program             */
	PRT_UINT32      transSetIndex;  /**< The index of the transition trigger set in program */
	PRT_UINT32      doSetIndex;     /**< The index of the do set in program                 */
	PRT_TRANSDECL   *transitions;   /**< The array of transitions                           */
	PRT_DODECL      *dos;           /**< The array of installed actions                     */
	PRT_UINT32      entryFunIndex;  /**< The index of entry function in owner machine       */
	PRT_UINT32      exitFunIndex;   /**< The index of exit function in owner machine        */

	PRT_UINT32      nAnnotations;   /**< Number of annotations                              */
	void            **annotations;  /**< An array of annotations                            */
} PRT_STATEDECL;

/** Represents a P machine declaration */
typedef struct PRT_MACHINEDECL
{
	PRT_UINT32       declIndex;         /**< The index of machine in program     */
	PRT_STRING       name;              /**< The name of this machine            */
	PRT_UINT32       nVars;             /**< The number of state variables       */
	PRT_UINT32       nStates;           /**< The number of states                */

	PRT_UINT32       maxQueueSize;      /**< The max queue size                  */
	PRT_UINT32       initStateIndex;    /**< The index of the initial state      */
	PRT_VARDECL      *vars;             /**< The array of variable declarations  */
	PRT_STATEDECL    *states;           /**< The array of state declarations     */
	PRT_SM_EXTCTOR   extCtor;           /**< external blob constructor           */
	PRT_SM_EXTDTOR   extDtor;           /**< external blob destructor            */

	PRT_UINT32      nAnnotations;   /**< Number of annotations                              */
	void            **annotations;  /**< An array of annotations                            */
} PRT_MACHINEDECL;

/** Represents a P model machine declaration */
typedef struct PRT_MODELIMPLDECL
{
	PRT_UINT32       declIndex;     /**< The index of model implementation in program       */
	PRT_STRING       name;          /**< The name of this machine                           */

	PRT_SM_MODELNEW      newFun;    /**< Function that creates instances of this machine    */
	PRT_SM_MODELSEND     sendFun;   /**< Function that sends to instances of this machine   */
	PRT_SM_MODELSHUTDOWN shutFun;   /**< Function that shuts down instances of this machine */

	PRT_UINT32      nAnnotations;   /**< Number of annotations                              */
	void            **annotations;  /**< An array of annotations                            */
} PRT_MODELIMPLDECL;

/** Represents a P program declaration */
typedef struct PRT_PROGRAMDECL
{
	PRT_UINT32      nEvents;      /**< The number of events      */
	PRT_UINT32      nEventSets;   /**< The number of event sets  */
	PRT_UINT32      nMachines;    /**< The number of machines    */
	PRT_UINT32      nModelImpls;  /**< The number of model implementations */

	PRT_EVENTDECL     *events;      /**< The array of events                 */
	PRT_EVENTSETDECL  *eventSets;   /**< The array of event set declarations */
	PRT_MACHINEDECL   *machines;    /**< The array of machines               */
	PRT_MODELIMPLDECL *modelImpls;  /**< The array of model implementations  */

	PRT_UINT32      nAnnotations;   /**< Number of annotations               */
	void            **annotations;  /**< An array of annotations             */
} PRT_PROGRAMDECL;

#ifdef __cplusplus
}
#endif
#endif
