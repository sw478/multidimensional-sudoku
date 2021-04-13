package org.sat4j.pb.constraints;

import org.sat4j.pb.IPBSolver;
import org.sat4j.pb.SolverFactory;

public class PBCPSkipMaxClauseCardConstrClauseLearningTest
        extends AbstractEZPseudoBooleanAndPigeonHoleTest {

    public PBCPSkipMaxClauseCardConstrClauseLearningTest(String arg) {
        super(arg);
    }

    @Override
    protected IPBSolver createSolver() {
        return SolverFactory.newCuttingPlanesStarClauseLearningSkip();
    }

}
