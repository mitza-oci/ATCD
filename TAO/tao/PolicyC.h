/* -*- C++ -*- */
// $Id$

// ****** Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO ORB and the TAO IDL Compiler have been developed by Washington
// University Computer Science's Distributed Object Computing Group.
//
// Information on TAO is available at
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#ifndef TAO_IDL_POLICYC_H
#define TAO_IDL_POLICYC_H

#include "tao/Object.h"
#include "tao/Sequence.h"
#include "tao/Sequence_T.h"

#if !defined (_CORBA_POLICY___PTR_CH_)
#define _CORBA_POLICY___PTR_CH_

class CORBA_Policy;
typedef CORBA_Policy *CORBA_Policy_ptr;

#endif /* end #if !defined */


#if !defined (_CORBA_POLICY___VAR_CH_)
#define _CORBA_POLICY___VAR_CH_

class TAO_Export  CORBA_Policy_var
{
public:
  CORBA_Policy_var (void); // default constructor
  CORBA_Policy_var (CORBA_Policy_ptr);
  CORBA_Policy_var (const CORBA_Policy_var &); // copy constructor
  ~CORBA_Policy_var (void); // destructor

  CORBA_Policy_var &operator= (CORBA_Policy_ptr);
  CORBA_Policy_var &operator= (const CORBA_Policy_var &);
  CORBA_Policy_ptr operator-> (void) const;

  operator const CORBA_Policy_ptr &() const;
  operator CORBA_Policy_ptr &();
  // in, inout, out, _retn
  CORBA_Policy_ptr in (void) const;
  CORBA_Policy_ptr &inout (void);
  CORBA_Policy_ptr &out (void);
  CORBA_Policy_ptr _retn (void);
  CORBA_Policy_ptr ptr (void) const;

private:
  CORBA_Policy_ptr ptr_;
};


#endif /* end #if !defined */


#if !defined (_CORBA_POLICY___OUT_CH_)
#define _CORBA_POLICY___OUT_CH_

class TAO_Export  CORBA_Policy_out
{
public:
  CORBA_Policy_out (CORBA_Policy_ptr &);
  CORBA_Policy_out (CORBA_Policy_var &);
  CORBA_Policy_out (CORBA_Policy_out &);
  CORBA_Policy_out &operator= (CORBA_Policy_out &);
  CORBA_Policy_out &operator= (const CORBA_Policy_var &);
  CORBA_Policy_out &operator= (CORBA_Policy_ptr);
  operator CORBA_Policy_ptr &();
  CORBA_Policy_ptr &ptr (void);
  CORBA_Policy_ptr operator-> (void);

private:
  CORBA_Policy_ptr &ptr_;
};


#endif /* end #if !defined */


#if !defined (_CORBA_POLICY_CH_)
#define _CORBA_POLICY_CH_

class TAO_Export  CORBA_Policy : public virtual ACE_CORBA_1 (Object)
{
public:
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_Policy_ptr _ptr_type;
  typedef CORBA_Policy_var _var_type;
#endif /* __GNUC__ */

  // the static operations
  static CORBA_Policy_ptr _duplicate (CORBA_Policy_ptr obj);
  static CORBA_Policy_ptr _narrow (CORBA::Object_ptr obj, CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  static CORBA_Policy_ptr _nil (void);

  virtual CORBA::PolicyType policy_type (
                                         CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()
                                         );
  virtual CORBA_Policy_ptr copy (
                                 CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()
                                 );
  virtual void destroy (
                        CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()
                        );
  virtual CORBA::Boolean _is_a (const CORBA::Char *type_id,
                                CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ()
                                );
  virtual const char* _interface_repository_id (void) const;
protected:
  CORBA_Policy (void); // default constructor
  CORBA_Policy (TAO_Stub *objref,
                TAO_ServantBase *_tao_servant = 0,
                CORBA::Boolean _tao_collocated = 0
                );
  virtual ~CORBA_Policy (void);
private:
  CORBA_Policy (const CORBA_Policy &);
  void operator= (const CORBA_Policy &);
};


#endif /* end #if !defined */


#if !defined (_CORBA_POLICYLIST_CH_)
#define _CORBA_POLICYLIST_CH_

// *************************************************************
// CORBA_PolicyList
// *************************************************************

class TAO_Export  CORBA_PolicyList : public TAO_Unbounded_Object_Sequence<CORBA_Policy>
{
public:
#if !defined(__GNUC__) || __GNUC__ > 2 || __GNUC_MINOR__ >= 8
  typedef CORBA_PolicyList_ptr _ptr_type;
  typedef CORBA_PolicyList_var _var_type;
#endif /* __GNUC__ */

  CORBA_PolicyList (void); // default ctor
  CORBA_PolicyList (CORBA::ULong max); // uses max size
  CORBA_PolicyList (
                    CORBA::ULong max,
                    CORBA::ULong length,
                    CORBA_Policy_ptr *buffer,
                    CORBA::Boolean release=0
                    );
  CORBA_PolicyList (const CORBA_PolicyList &); // copy ctor
  ~CORBA_PolicyList (void); // dtor
};
typedef CORBA_PolicyList *CORBA_PolicyList_ptr;
// not used: static CORBA::TypeCode_ptr _tc_PolicyList_seq;


#endif /* end #if !defined */


#if !defined (_CORBA_POLICYLIST___VAR_CH_)
#define _CORBA_POLICYLIST___VAR_CH_

// *************************************************************
// class CORBA::PolicyList_var
// *************************************************************

class TAO_Export  CORBA_PolicyList_var
{
public:
  CORBA_PolicyList_var (void); // default constructor
  CORBA_PolicyList_var (CORBA_PolicyList *);
  CORBA_PolicyList_var (const CORBA_PolicyList_var &); // copy constructor
  ~CORBA_PolicyList_var (void); // destructor

  CORBA_PolicyList_var &operator= (CORBA_PolicyList *);
  CORBA_PolicyList_var &operator= (const CORBA_PolicyList_var &);
  CORBA_PolicyList *operator-> (void);
  const CORBA_PolicyList *operator-> (void) const;

  operator const CORBA_PolicyList &() const;
  operator CORBA_PolicyList &();
  operator CORBA_PolicyList &() const;
  TAO_Object_Manager <CORBA_Policy > operator[] (CORBA::ULong index);
  // in, inout, out, _retn
  const CORBA_PolicyList &in (void) const;
  CORBA_PolicyList &inout (void);
  CORBA_PolicyList *&out (void);
  CORBA_PolicyList *_retn (void);
  CORBA_PolicyList *ptr (void) const;

private:
  CORBA_PolicyList *ptr_;
};


#endif /* end #if !defined */


#if !defined (_CORBA_POLICYLIST___OUT_CH_)
#define _CORBA_POLICYLIST___OUT_CH_

class TAO_Export  CORBA_PolicyList_out
{
public:
  CORBA_PolicyList_out (CORBA_PolicyList *&);
  CORBA_PolicyList_out (CORBA_PolicyList_var &);
  CORBA_PolicyList_out (CORBA_PolicyList_out &);
  CORBA_PolicyList_out &operator= (CORBA_PolicyList_out &);
  CORBA_PolicyList_out &operator= (CORBA_PolicyList *);
  operator CORBA_PolicyList *&();
  CORBA_PolicyList *&ptr (void);
  CORBA_PolicyList *operator-> (void);
  TAO_Object_Manager <CORBA_Policy > operator[] (CORBA::ULong index);

private:
  CORBA_PolicyList *&ptr_;
  // assignment from T_var not allowed
  void operator= (const CORBA_PolicyList_var &);
};

#endif /* end #if !defined */

#if defined (__ACE_INLINE__)
#include "PolicyC.i"
#endif /* defined INLINE */

#endif /* TAO_IDL_POLICYC_H */
