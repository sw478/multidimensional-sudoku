package org.sat4j.br4cp;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.sat4j.core.VecInt;
import org.sat4j.specs.ContradictionException;
import org.sat4j.specs.ISolver;
import org.sat4j.specs.IVecInt;
import org.sat4j.specs.IteratorInt;
import org.sat4j.specs.TimeoutException;
import org.sat4j.tools.Backbone;

/**
 * A class used to compile propagation in configuration problem due to
 * assumptions.
 * 
 * @author lonca
 * @author leberre
 */
public class DefaultBr4cpBackboneComputer implements IBr4cpBackboneComputer {

	private ISolver solver;
	private ConfigVarMap varMap;
	private List<Set<Integer>> solverAssumptions = new ArrayList<Set<Integer>>();
	private Set<String> fixedVars = new HashSet<String>();

	private Set<String> propagatedConfigVars;
	private Set<String> domainReductions;
	private Set<String> propagatedAdditionalVars;
	private Set<String> unavailableAdditionalVars;

	private IVecInt filter;

	public DefaultBr4cpBackboneComputer(ISolver solver, ConfigVarMap varMap)
			throws TimeoutException {
		this.solver = solver;
		this.varMap = varMap;
		filter = new VecInt(solver.nVars());
		for (int i = 1; i <= solver.nVars(); i++) {
			if (varMap.isConfigVar(i) || varMap.isAdditionalVar(i)) {
				filter.push(i);
			}
		}
		computeBackbone(solver);
	}

	protected void computeBackbone(ISolver solver) throws TimeoutException {
		IVecInt assumps = new VecInt();
		for (Iterator<Set<Integer>> it = solverAssumptions.iterator(); it
				.hasNext();) {
			for (Iterator<Integer> it2 = it.next().iterator(); it2.hasNext();)
				assumps.push(it2.next());
		}
		try {
			IVecInt backbone = Backbone.instance().compute(solver, assumps,
					filter);
			computePropagationsAndReductions(backbone);
		} catch (IllegalArgumentException ise) {
			ise.printStackTrace();
			if (this.solverAssumptions.isEmpty()) {
				throw ise;
			}
			Set<Integer> removed = this.solverAssumptions
					.remove(this.solverAssumptions.size() - 1);
			for (Iterator<Integer> it = removed.iterator(); it.hasNext();) {
				String nextVar = this.varMap.getConfigVar(Math.abs(it.next()));
				this.fixedVars.remove(nextVar.substring(0,
						nextVar.lastIndexOf('_')));
			}
			throw new IllegalArgumentException(
					"last assumption implies a contradiction");
		}
	}

	private void computePropagationsAndReductions(IVecInt backbone) {
		this.propagatedConfigVars = new TreeSet<String>(
				new ConfigVarComparator());
		this.domainReductions = new TreeSet<String>(new ConfigVarComparator());
		this.propagatedAdditionalVars = new TreeSet<String>(
				new ConfigVarComparator());
		this.unavailableAdditionalVars = new TreeSet<String>(
				new ConfigVarComparator());
		for (IteratorInt it = backbone.iterator(); it.hasNext();) {
			int next = it.next();
			if (this.varMap.isAdditionalVar(next)) {
				String name = this.varMap.getConfigVar(Math.abs(next));
				assert name != null;
				if (next < 0) {
					this.unavailableAdditionalVars.add(name);
				} else {
					this.propagatedAdditionalVars.add(name);
				}
			} else if ((next > 0) && this.varMap.isConfigVar(next)) {
				String name = this.varMap.getConfigVar(next);
				int lastDotIndex = name.lastIndexOf('_');
				name = name.substring(0, lastDotIndex) + "="
						+ name.substring(lastDotIndex + 1);
				this.fixedVars.add(name.substring(0, lastDotIndex));
				this.propagatedConfigVars.add(name);
			} else if ((next < 0) && this.varMap.isConfigVar(next)) {
				String name = this.varMap.getConfigVar(-next);
				int lastDotIndex = name.lastIndexOf('_');
				name = name.substring(0, lastDotIndex) + "="
						+ name.substring(lastDotIndex + 1);
				this.domainReductions.add(name);
			}
		}
		for (Iterator<String> it = this.domainReductions.iterator(); it
				.hasNext();) {
			String next = it.next();
			if (this.fixedVars
					.contains(next.substring(0, next.lastIndexOf('=')))) {
				it.remove();
			}
		}
	}

	public void addAssumption(String configVar) throws TimeoutException,
			ContradictionException {
		if (this.varMap.configVarExists(configVar)) {
			int lastDotIndex = configVar.lastIndexOf('_');
			String configVarName = configVar.substring(0, lastDotIndex);
			if (this.fixedVars.contains(configVarName)
					&& Options.getInstance().areReplacementAllowed()) {
				removeAssumedConfigVar(configVarName);
			}
			Set<Integer> newAssumpSet = new HashSet<Integer>();
			newAssumpSet.add(this.varMap.getSolverVar(configVar));
			this.solverAssumptions.add(newAssumpSet);
			this.fixedVars.add(configVarName);
			computeBackbone(solver);
		} else {
			throw new IllegalArgumentException(configVar + " is not defined");
		}
	}

	private void removeAssumedConfigVar(String configVarName) {
		this.fixedVars = new HashSet<String>();
		for (Iterator<Set<Integer>> it = this.solverAssumptions.iterator(); it
				.hasNext();) {
			Set<Integer> nextSet = it.next();
			for (Iterator<Integer> it2 = nextSet.iterator(); it2.hasNext();) {
				String nextVar = this.varMap.getConfigVar(Math.abs(it2.next()));
				nextVar = Utils.extractName(nextVar);
				if (configVarName.equals(nextVar)) {
					it2.remove();
					if (nextSet.isEmpty()) {
						it.remove();
					}
					break;
				} else {
					this.fixedVars.add(nextVar);
				}
			}
		}
	}

	public void setOptionalConfigVarAsNotInstalled(String optConfigVar)
			throws TimeoutException {
		Set<Integer> newAssumps = new HashSet<Integer>();
		if (this.varMap.isAdditionalVar(optConfigVar)) {
			newAssumps.add(-this.varMap.getSolverVar(optConfigVar));
			this.fixedVars.add(optConfigVar);
		} else {
			Set<String> domain = this.varMap.getConfigVarDomain(optConfigVar);
			for (String s : domain) {
				Valeur valeur = Utils.extractValeur(s);
				if (valeur.valeur != Utils.JOKER) {
					newAssumps.add(-this.varMap.getSolverVar(s));
				}
			}
			this.fixedVars.add(optConfigVar);
		}
		this.solverAssumptions.add(newAssumps);
		computeBackbone(solver);
	}

	public void addAdditionalVarAssumption(String addVar)
			throws TimeoutException {
		Valeur valeur = Utils.extractValeur(addVar);
		if (valeur.valeur == -1) {
			throw new NumberFormatException(addVar + " has no version or state");
		}
		Set<Integer> newAssumpSet = new HashSet<Integer>();
		if (valeur.valeur == Utils.JOKER) {
			newAssumpSet.add(-this.varMap.getSolverVar(valeur.variable));
		} else {
			newAssumpSet.add(this.varMap.getSolverVar(valeur.variable));
		}
		this.solverAssumptions.add(newAssumpSet);
		this.fixedVars.add(valeur.variable);
		computeBackbone(solver);
	}

	public void clearAssumptions() {
		this.solverAssumptions.clear();
		this.fixedVars.clear();
		try {
			computeBackbone(solver);
		} catch (TimeoutException e) {
			throw new RuntimeException(e);
		}
	}

	public Set<String> propagatedConfigVars() {
		return this.propagatedConfigVars;
	}

	public Set<String> domainReductions() {
		return this.domainReductions;
	}

	public Set<String> propagatedAdditionalVars() {
		return this.propagatedAdditionalVars;
	}

	public Set<String> unavailableAdditionalVars() {
		return this.unavailableAdditionalVars;
	}

	public List<Set<Integer>> getSolverAssumptions() {
		return this.solverAssumptions;
	}

	public int getNumberOfSATCalls() {
		return Backbone.instance().getNumberOfSatCalls();
	}

	public boolean isPresentInCurrentDomain(String var, String val) {
		return !this.domainReductions.contains(var + "=" + val)
				&& (!fixedVars.contains(var) || propagatedConfigVars
						.contains(var + "=" + val));
	}

	public int getSizeOfCurrentDomainOf(String var) {
		return getCurrentDomainOf(var).size();
	}

	public Set<String> getCurrentDomainOf(String var) {
		Set<String> domain = new HashSet<String>();
		String value;
		Set<String> originalDomain = varMap.getDomain(var);
		assert originalDomain != null;
		for (Iterator<String> it = originalDomain.iterator(); it.hasNext();) {
			value = it.next();
			Valeur valeur = Utils.extractValeur(value);
			// for Series, Pack, etc.
			if (unavailableAdditionalVars.contains(var)) {
				domain.clear();
				domain.add(String.valueOf(Utils.JOKER));
				break;
			}
			// for optional vx_0
			if (unavailableAdditionalVars.contains(value)) {
				domain.clear();
				domain.add(String.valueOf(Utils.JOKER));
				break;
			}
			if (propagatedAdditionalVars.contains(var)
					&& valeur.valeur != Utils.JOKER) {
				domain.clear();
				domain.add(String.valueOf(valeur.valeur));
				break;
			}
			if (propagatedAdditionalVars.contains(value)) {
				domain.clear();
				domain.add(String.valueOf(valeur.valeur));
				break;
			}
			if (propagatedConfigVars.contains(value)) {
				domain.clear();
				domain.add(String.valueOf(valeur.valeur));
				break;
			}
			if (!domainReductions.contains(valeur.toString())) {
				domain.add(String.valueOf(valeur.valeur));
			}
		}
		assert !fixedVars.contains(var) || (domain.size() == 1) : fixedVars
				+ "/" + var + domain;
		return domain;
	}

	public void unassign(String var) throws TimeoutException {
		removeAssumedConfigVar(var);
		computeBackbone(solver);
	}

	public Set<String> getFreeVariables() {
		Set<String> all = varMap.getVars();
		Set<String> free = new HashSet<String>();
		for (String var : all) {
			if (getCurrentDomainOf(var).size() > 1) {
				free.add(var);
			}
		}
		return free;
	}
}
